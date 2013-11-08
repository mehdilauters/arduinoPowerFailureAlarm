/**************************************************************************

  Nokia 61xx SMS send/receive functions

  Version 1.0

  Justin Karneges
  infiniti@affinix.com
  June 2000

  Thanks goes to the gnokii team for figuring out the frame format, among
  many other things!  These guys did all the hard work.  Although this file
  was written from scratch, it was heavily based on their research.

  http://www.gnokii.org/

  Overview:

    This file contains a set of functions for sending and receiving SMS
    (short message service) text messages across a GSM network with a
    Nokia 61xx series phone.  I've only tested it with my phone (a 6190),
    however it should work fine with any GSM phone in the series.  It
    should also work with the 51xx series phones.

    These functions were meant to be as portable as possible (they contain
    no serial communications code), and to perform two simple things: send
    and receive SMS.  That's all.  If you want a full program to control
    all the aspects of the Nokia 61xx, then I suggest checking out gnokii.
    This code was meant to be small, so that it could be used in small
    devices/applications.


  To use:

    Setup the serial port and call n61_init().  Then use n61_smssend() and
    n61_smsrecv() to send/recv SMS messages.  Call n61_update() as often
    as possible (possibly by having a timer ISR call it) so that the driver
    stays in sync with the phone.  All incoming SMS messages are deleted
    from the phone's inbox (you want this).


  ------------------------------------------------------------------------
  Functions:

    +---------------------------------------------------------+
    |  bool n61_init(int (*func)(int cmd, unsigned char c));  |
    +---------------------------------------------------------+

      Before calling this function, do everything necessary to have the
      serial port ready.  The port MUST be set as follows:

        Baud rate: 115200
        Parity: None
        Bits: 8
        Stop bits: 1
        DTR: set
        RTS: cleared

      Since this driver does not contain any serial communications
      functionality, you must supply it.  An interrupt driven / threaded
      serial interface MUST be used.  This driver does not poll the port,
      and so it is completely dependant on a background serial driver
      keeping a queue of all incoming data (so that no data is lost).

      To give the serial functionality to this nokia driver, write a simple
      function to perform the four types of requests that this driver will
      need handled.  Pass this function as the argument to init:

        n61_init(myhandler);

      The myhandler() function should look like this:

        int myhandler(int cmd, unsigned char c)
        {
            if(cmd == 0) {
                serial_send(c);          // send c out the port
                return;
            }
            else if(cmd == 1)
                return serial_recv();    // return the next byte in queue
            else if(cmd == 2)
                return serial_isdata();  // return zero if queue is empty
            else if(cmd == 3)
                msleep(1);               // delay for 1 millisecond
        }

      0 means send "c" out the serial port.  1 means return a byte from the
      serial port.  2 means return true/false if there is data waiting.
      Simple enough!

      This driver also requires a millisecond resolution timer, which is
      what the last request is for.  Most platforms include some sort of
      millisecond (or less) delay function.  For MSVC++ there is Sleep(),
      Unix has usleep() and DOS has delay().  If you're not using such a
      platform, then you'll have to time it yourself.  The driver doesn't
      specify how many milliseconds to wait, your function should wait
      just one millisecond.  So just do Sleep(1), usleep(1000), delay(1),
      or whatever your homebrew method is.  Easy!

      Lastly, n61_init() also queries the phone for the SMSC number to be
      used when sending SMS.

      Whew!  If you got past n61_init(), then the rest is easy as cake.

    +--------------------------------------------+
    |  int n61_smssend(char *dest, char *msg);  |
    +--------------------------------------------+

      Sends "msg" to "dest".  Returns 1 if sent, 0 if not.


    +---------------------------------------------+
    |  int n61_smsrecv(char *source, char *msg);  |
    +---------------------------------------------+

      Copies a received message into "msg", stores the source phone number
      in "source" and returns 1.  Returns 0 if no messages are in the queue.
      "msg" will not be larger than 161 bytes (including null byte).
      "source" will not be larger than 17 bytes (including null byte).


    +-----------------------+
    |  int n61_smsqueue();  |
    +-----------------------+

      Returns the number of messages in the incoming queue.


    +---------------------+
    |  int n61_update();  |
    +---------------------+

      This must be called as often as possible.  If it's not called, then
      you won't be able to receive messages.  This might be something good
      to put in a timer interrupt if possible (yes, it's safe).


  That's all you need to know!

  ------------------------------------------------------------------------

  Nokia 6190 message format:

    1 byte = start byte (0x1e)
    1 byte = message destination
    1 byte = message source
    1 byte = message type
    1 byte = ???
    1 byte = message size

    X bytes = message data (X = message size)

    1 byte = filler byte (exists only if needed to make message size even)

    2 byte = result of all 16bit words XOR'ed together

**************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "PowerFailureAlarm.h"



#define NB6_HEADER_SIZE 6


#define N61MODE_SYNC  0
#define N61MODE_DEST  1
#define N61MODE_SOURCE  2
#define N61MODE_TYPE  3
#define N61MODE_UNKNOWN  4
#define N61MODE_SIZE  5
#define N61MODE_DATA  6

#define N61STATE_WAIT  0
#define N61STATE_PROC  1
#define N61STATE_GOOD  2
#define N61STATE_BAD  3

#define N61MAXSIZE  120

#define N61VALID_1H  0x0b
#define N61VALID_6H  0x47
#define N61VALID_24H  0xa7
#define N61VALID_72H  0xa9
#define N61VALID_1W  0xad
#define N61VALID_MAX  0xff

struct N61_MESSAGE
{
  int dest;
  int source;
  int type;
  int unknown;
  int size;
  unsigned char dat[256];
  int cs[2];
};

struct N61_MESSAGE n61_tmp, n61_buf;
int n61_seqnum = 0;
int n61_prevseq = 0;
int (*n61_serial)(int cmd, unsigned char c);
int n61_mode = 0;
int n61_atbyte = 0;
int n61_datp = 0;
int n61_readsize = 0;
int n61_readbase = 0;
int n61_state;
int n61_multiple;
char n61_smsc[32];
int n61_msgqueuesize;
// TODO
//char n61_msgqueue[4][161];
//char n61_msgqueue2[4][17];
int n61_ack, n61_gotmsg, n61_waittype;
int n61_blocked;


// ** translate table taken directly from gnokii **

unsigned char n61_transtable[] = {

  /* ETSI GSM 03.38, version 6.0.1, section 6.2.1; Default alphabet */
  /* Characters in hex position 10, [12 to 1a] and 24 are not present on
    latin1 charset, so we cannot reproduce on the screen, however they are
    greek symbol not present even on my Nokia */

  '@',  0xa3, '$',  0xa5, 0xe8, 0xe9, 0xf9, 0xec,
  0xf2, 0xc7, '\n', 0xd8, 0xf8, '\r', 0xc5, 0xe5,
  '?',  '_',  '?',  '?',  '?',  '?',  '?',  '?',
  '?',  '?',  '?',  '?',  0xc6, 0xe6, 0xdf, 0xc9,
  ' ',  '!',  '\"', '#',  0xa4,  '%',  '&',  '\'',
  '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
  0xa1, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
  'X',  'Y',  'Z',  0xc4, 0xd6, 0xd1, 0xdc, 0xa7,
  0xbf, 'a',  'b',  'c',  'd',  'e',  'f',  'g',
  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
  'x',  'y',  'z',  0xe4, 0xf6, 0xf1, 0xfc, 0xe0
};



// **** Functions            ****

bool n61_init(int (*func)(int cmd, unsigned char c));  // initialize the driver
int n61_smssend(char *dest, char *msg);      // send SMS
int n61_smsrecv(char *dest, char *msg);      // recv SMS
int n61_smsqueue();          // check the recv queue
void n61_update();          // keep the driver in sync

// **** Functions - internal ****

void n61_block();            // protect during update
void n61_unblock();            // unprotect

void n61_update_main();            // main update
void n61_update_internal();          // internal version of
                //   n61_update()

void n61_serial_send(unsigned char c);        // send byte
unsigned char n61_serial_recv();        // recv byte
unsigned char n61_serial_isdata();        // check for serial data
void n61_serial_delay();          // wait 1 millisecond

int n61_nextseq();            // get next sequence number
int n61_sendframe(int type, int size, unsigned char *data);  // send frame
int n61_sendmsg(int type, int size, unsigned char *data);  // send message
void n61_sendack(int type, int seqnum);        // send acknowledgement

void n61_ackwait(int x);          // wait for ack
void n61_wait(int x);            // wait for a message

void n61_addchar(char *str, char c);        // strcat() but for chars
void n61_bcd(unsigned char *dest, char *s);        // encode SMSC number
void n61_bcd2(unsigned char *dest, char *s);        // encode phone number
char *n61_unbcd(unsigned char *dat);        // decode SMSC number
char *n61_unbcd2(unsigned char *dat);        // decode phone number

void n61_pack7(unsigned char *dest, char *s);        // pack when sending
char *n61_unpack7(unsigned char *dat, int len);      // unpack when received

unsigned char n61_gettrans(unsigned char c);      // translate char
void n61_addmsg(char *dest, char *msg);        // add a received SMS
                //   to the queue

void n61_procmsg(struct N61_MESSAGE *msg);      // process incoming message

void n61_getstatus();            // request phone status
bool n61_getsmsc();            // request SMSC
void n61_delsms(int x);            // delete SMS message
int n61_smssendfull(char *smsc, char *dest, char *msg);    // send SMS via SMSC


// ** Code **
unsigned char n61_gettrans(unsigned char c)
{
  unsigned char n;

  if(c == '?')
    return 0x3f;

  for(n = 0; n < 128; ++n) {
    if(n61_transtable[n] == c)
      return n;
  }

  return 0x3f;
}

void n61_block()
{
  n61_blocked = 1;
}

void n61_unblock()
{
  n61_blocked = 0;
}

void n61_update()
{
  if(!n61_blocked) {
    n61_block();
    n61_update_main();
    n61_unblock();
  }
}

void n61_update_internal()
{
  n61_update_main();
}

void n61_serial_send(unsigned char c)
{
  n61_serial(0, c);
}

unsigned char n61_serial_recv()
{
  return n61_serial(1, 0);
}

unsigned char n61_serial_isdata()
{
  return n61_serial(2, 0);
}

void n61_serial_delay()
{
  n61_serial(3, 0);
}

void n61_ackwait(int x)
{
  int n;

  for(n = 0; n < x; ++n) {
    n61_update_internal();
    n61_serial_delay();
    if(n61_ack)
      return;
  }
}

void n61_wait(int x)
{
  int n;

  for(n = 0; n < x; ++n) {
    n61_update_internal();
    n61_serial_delay();
    if(n61_gotmsg)
      return;
  }
}


int n61_sendframe(int type, int size, unsigned char *data)
{
  //unsigned char buf[256];

  unsigned char header[NB6_HEADER_SIZE];

  int at =0, n, check, check1;


  // build header
  header[at++] = 0x1e;    // message startbyte
  header[at++] = 0x00;    // dest: phone
  header[at++] = 0x0c;    // source: PC
  header[at++] = type;
  header[at++] = 0x00;
  header[at++] = size;

  // calculate checksums
  check = 0;
  check1 = 0;

  at = 0;

  //send header
  for(n = 0; n < NB6_HEADER_SIZE; ++n) {
    n61_serial_send(header[n]);
    if( at % 2 == 0)
        check ^= header[n];
    else
        check1 ^= header[n];
    at++;
  }

  //send data
  for(n = 0; n < size; ++n) {
    n61_serial_send(data[n]);
    if( at % 2 == 0)
        check ^= data[n];
    else
        check1 ^= data[n];
    at++;
  }

  // send data filler if needed
  if(size % 2) {
    n61_serial_send(0x00);
    if( at % 2 == 0)
        check ^= 0x00;
    else
        check1 ^= 0x00;
    at++;
  }

  //send checksums
  n61_serial_send(check);
  at++;

  if( at % 2 == 0)
        check1 ^= check;

  n61_serial_send(check1);

  return 0;
}

int n61_nextseq()
{
  int n;

  n = n61_seqnum;
  n61_prevseq = n;

  ++n61_seqnum;
  n61_seqnum &= 7;

  return (n + 0x40);
}

int n61_sendmsg(int type, int size, unsigned char *data)
{
  //unsigned char buf[N61MAXSIZE + 2];
  unsigned char *buf;
  unsigned char num, lastsize;
  int n;
  int len;
  int msgSize = 0;
  num = (size + N61MAXSIZE - 1) / N61MAXSIZE;
  lastsize = size % N61MAXSIZE;

  for(n = 0; n < num;) {
    if(n + 1 == num)
      len = lastsize;
    else
      len = N61MAXSIZE;

    msgSize = len + 2;
    buf = (unsigned char*) malloc( msgSize * sizeof(char) );
    // get current chunk
    memcpy(buf, data + (n * N61MAXSIZE), len);


    buf[len] = num - n;
    buf[len+1] = n61_nextseq();
    if(n)
      buf[len+1] &= 7;

    n61_ack = 0;
    bool sent = false;
    for(int p=0; p<FBUS_NB_TRIES; p++)
    {
		n61_sendframe(type, msgSize, buf);
		n61_ackwait(1000);
	    if(n61_ack)
	    {
	    	sent = true;
	      ++n;
	    }
    }
    free(buf);
    if(!sent)
    {
    	break;
    }
  }
  return 0;
}

void n61_sendack(int type, int seqnum)
{
  unsigned char buf[2];

  buf[0] = type;
  buf[1] = seqnum;

  n61_sendframe(0x7f, 2, buf);
}

void n61_update_main()
{
  int n;
  unsigned char c;

  while(n61_serial_isdata()) {
    c = n61_serial_recv();
    // calculate the checksums
    n61_tmp.cs[n61_atbyte & 1] ^= c;

    // act on the byte
    switch(n61_mode) {
      case N61MODE_SYNC:
        if(c == 0x1e) {
          if(!n61_multiple) {
            memset(n61_tmp.dat, 0, 256);
            n61_atbyte = 0;
          }

          n61_tmp.cs[0] = 0x1e;
          n61_tmp.cs[1] = 0;

          n61_mode = N61MODE_DEST;
        }
        break;

      case N61MODE_DEST:
        n61_tmp.dest = c;
        n61_mode = N61MODE_SOURCE;
        break;

      case N61MODE_SOURCE:
        n61_tmp.source = c;
        n61_mode = N61MODE_TYPE;
        break;

      case N61MODE_TYPE:
        n61_tmp.type = c;
        n61_mode = N61MODE_UNKNOWN;
        break;

      case N61MODE_UNKNOWN:
        n61_tmp.unknown = c;
        n61_mode = N61MODE_SIZE;
        break;

      case N61MODE_SIZE:
        if(n61_multiple)
          n61_tmp.size += c - 2;
        else {
          n61_tmp.size = c;
          n61_datp = 0;
        }

        n61_mode = N61MODE_DATA;

        // calculate the number of bytes to read
        n = n61_tmp.size % 2;

        // message size + filler + checksums
        n61_readsize = n61_tmp.size + n + 2;

        break;

      case N61MODE_DATA:
        n = n61_datp++;
        if(n > 255) {
          n61_multiple = 0;
          n61_mode = N61MODE_SYNC;
          break;
        }

        n61_tmp.dat[n] = c;  // get the byte

        // are we done yet?
        if(n >= n61_readsize - 1) {
          // checksums ok?
          if(n61_tmp.cs[0] == n61_tmp.cs[1] && n61_tmp.cs[0] == 0) {
            // don't want to ACK on an ACK
            if(n61_tmp.type != 0x7f) {
              n61_sendack(n61_tmp.type, n61_tmp.dat[n61_tmp.size-1] & 0x0f);

              if(n61_tmp.size > 1 && n61_tmp.dat[n61_tmp.size-2] != 0x01) {
                n61_datp -= 4;  // back up past checksums and seqinfo
                ++n61_multiple;
              }
              else
                n61_multiple = 0;
            }

            if(!n61_multiple || n61_tmp.type == 0x7f) {
              n61_multiple = 0;
              memcpy(&n61_buf, &n61_tmp, sizeof(struct N61_MESSAGE));
              n61_procmsg(&n61_buf);
            }
          }
          else {
            // bad!
            n61_multiple = 0;
          }
          n61_mode = N61MODE_SYNC;
        }

      default:
        break;
    }

    ++n61_atbyte;
  }
}

void n61_getstatus()
{
  unsigned char buf[32];

  buf[0] = 0x00;
  buf[1] = 0x01;
  buf[2] = 0x00;

  buf[3] = 0x6d;

  n61_sendmsg(0x11, 4, buf);
}

bool n61_getsmsc()
{
  unsigned char buf[32];

  buf[0] = 0x00;
  buf[1] = 0x01;
  buf[2] = 0x00;

  buf[3] = 0x33;
  buf[4] = 0x64;

  buf[5] = 0x01;

  // "do" or "try", there is no "do not"
    for(int n=0; n<GETSMSC_NB_TRIES; n++)
    {
    // send off the request
    n61_state = N61STATE_PROC;

    n61_gotmsg = 0;
    n61_waittype = 2;

    n61_sendmsg(0x02, 6, buf);

    n61_wait(2000);
    if(n61_state == N61STATE_GOOD)
    {
      return true;
    }
  }
  return false;
}


void n61_bcd(unsigned char *dest, char *s)
{
  int x, y, n, hi, lo;

  if(s[0] == '+') {
    dest[1] = 0x91;
    ++s;
  }
  else
    dest[1] = 0x81;

  x = 0;
  y = 2;
  while(s[x]) {
    lo = s[x++] - '0';
    if(s[x])
      hi = s[x++] - '0';
    else
      hi = 0x0f;

    n = (hi << 4) + lo;
    dest[y++] = n;
  }
  dest[0] = y - 1;
}

void n61_bcd2(unsigned char *dest, char *s)
{
  int x, y, n, hi, lo;

  if(s[0] == '+') {
    dest[1] = 0x91;
    ++s;
  }
  else
    dest[1] = 0x81;

  x = 0;
  y = 2;
  while(s[x]) {
    lo = s[x++] - '0';
    if(s[x])
      hi = s[x++] - '0';
    else
      hi = 0x0f;

    n = (hi << 4) + lo;
    dest[y++] = n;
  }
  dest[0] = strlen(s);
}

void n61_pack7(unsigned char *dest, char *s)
{
  int len;
  unsigned short *p, w;
  int at;
  int shift;
  int n, x;

  len = strlen(s);
  x = (len * 8) / 7;
  for(n = 0; n < x; ++n)
    dest[n] = 0;

  shift = 0;
  at = 0;
  w = 0;
  for(n = 0; n < len; ++n) {
    p = (unsigned short *)(dest + at);
    w = n61_gettrans(s[n]) & 0x7f;
    w <<= shift;

    *p |= w;

    shift += 7;
    if(shift >= 8) {
      shift &= 7;
      ++at;
    }
  }
}

int n61_smssendfull(char *smsc, char *dest, char *msg)
{
  //unsigned char buf[256];
  unsigned char *buf;
  int n, smsFrameSize, msgLen;

  msgLen = strlen(msg);
  smsFrameSize = 42 + msgLen;
  buf = (unsigned char *) malloc( smsFrameSize * sizeof(char));
  // standard frame data header
  buf[0] = 0x00;
  buf[1] = 0x01;
  buf[2] = 0x00;

  // send sms ?
  buf[3] = 0x01;
  buf[4] = 0x02;
  buf[5] = 0x00;

  // smsc
  memset(buf + 6, 0, 12);
  n61_bcd(buf + 6, smsc);

  // TPDU ?
  buf[18] = 0x11;

  // misc
  buf[19] = 0x00;  // message ref
  buf[20] = 0x00;  // protocol ID
  buf[21] = 0xf1;  // data coding scheme (non-flash)

  // message size
  buf[22] = msgLen;

  // destination
  memset(buf + 23, 0, 12);
  n61_bcd2(buf + 23, dest);

  // validity period
  buf[35] = N61VALID_24H;

  // filler
  buf[36] = 0;
  buf[37] = 0;
  buf[38] = 0;
  buf[39] = 0;
  buf[40] = 0;
  buf[41] = 0;

  // the string
  n61_pack7(buf + 42, msg);
  // try till we get some response
  for(n = 0; n< SMS_SEND_NB_TRIES; n++) {
    n61_state = N61STATE_PROC;

    n61_gotmsg = 0;
    n61_waittype = 1;
    n61_sendmsg(0x02, smsFrameSize, buf);

    n61_wait(5000);
    if(n61_state != N61STATE_PROC)
      break;
  }
  free(buf);
  if(n61_state == N61STATE_GOOD)
    return 1;

  return 0;
}

int n61_smssend(char *dest, char *msg)
{
  int n;

  n61_block();
  n = n61_smssendfull(n61_smsc, dest, msg);
  n61_unblock();

  return n;
}

void n61_addchar(char *str, char c)
{
  int n;

  n = strlen(str);
  str[n] = c;
  str[n+1] = 0;
}

char *n61_unbcd(unsigned char *dat)
{
  static char buf[32];
  int len;
  int n, x;

  buf[0] = 0;
  len = dat[0];

  if(dat[1] == 0x91) {
    n61_addchar(buf, '+');
  }

  for(n = 0; n < len-1; ++n) {
    x = dat[n+2] & 0x0f;
    if(x < 10)
      n61_addchar(buf, '0' + x);
    x = (dat[n+2] >> 4) & 0x0f;
    if(x < 10)
      n61_addchar(buf, '0' + x);
  }

  return buf;
}

char *n61_unbcd2(unsigned char *dat)
{
  static char buf[32];
  int len;
  int n, x;
  int at;

  buf[0] = 0;
  len = dat[0];

  if(dat[1] == 0x6f || dat[1] == 0x91) {
    n61_addchar(buf, '+');
  }

  at = 2;
  for(n = 0; n < len; ++n) {
    x = dat[at] & 0x0f;
    if(x < 10)
      n61_addchar(buf, '0' + x);
    ++n;
    if(!(n < len))
      break;
    x = (dat[at] >> 4) & 0x0f;
    if(x < 10)
      n61_addchar(buf, '0' + x);
    ++at;
  }

  return buf;
}

char *n61_unpack7(unsigned char *dat, int len)
{
  static char buf[256];
  unsigned short *p, w;
  unsigned char c;
  int n;
  int shift;
  int at;

  shift = 0;
  at = 0;
  buf[0] = 0;
  for(n = 0; n < len; ++n) {
    p = (unsigned short *)(dat + at);
    w = *p;
    w >>= shift;
    c = w & 0x7f;

    shift += 7;
    if(shift & 8) {
      shift &= 0x07;
      ++at;
    }

    n61_addchar(buf, n61_transtable[c]);
  }

  return buf;
}

void n61_procmsg(struct N61_MESSAGE *msg)
{
  int subtype;

  // check if this is a msg of interest
  subtype = 0;
  if(msg->type != 0x7f) {
    if(msg->type == 0x02) {
      if(msg->dat[3] == 0x02 || msg->dat[3] == 0x03)
        subtype = 1;
      if(msg->dat[3] == 0x10)
        subtype = 2;
      if(msg->dat[3] == 0x34)
        subtype = 3;
    }
    if(subtype == n61_waittype)
      n61_gotmsg = 1;
  }

  // act on it
  switch(msg->type) {
//    // SMS
    case  0x02:
      if(msg->dat[3] == 0x02) {
        n61_state = N61STATE_GOOD;
      }
      if(msg->dat[3] == 0x03) {
        n61_state = N61STATE_BAD;
      }
//      if(msg->dat[3] == 0x10) {
//        n61_addmsg(n61_unbcd2(msg->dat + 23), n61_unpack7(msg->dat + 42, msg->dat[22]));
//
//        // now delete the msg
//        if(msg->dat[5])
//          n61_delsms(msg->dat[5]);
//      }
//
//      // SMSC
      if(msg->dat[3] == 0x34) {
        n61_state = N61STATE_GOOD;
        strcpy(n61_smsc, n61_unbcd(msg->dat+21));
      }
      break;
//
    case  0x7f:
      if((msg->dat[1] & 7) == n61_prevseq)
        n61_ack = 1;
      break;

    default:
      break;
  }
}

bool n61_init(int (*func)(int cmd, unsigned char c))
{

  n61_seqnum = 0;
  n61_serial = func;
  n61_blocked = 0;
  n61_msgqueuesize = 0;
  n61_multiple = 0;
  n61_readbase = 0;

  // getsmsc
  return n61_getsmsc();
}
