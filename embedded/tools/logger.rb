#!/usr/bin/ruby

require 'rubygems'
require 'serialport'

require 'term/ansicolor'
include Term::ANSIColor
#gem install term-ansicolor


# 
# 
# 
# require 'Qt'
# 
# 
# class QtLogger < Qt::Widget
# 
#     def initialize(sp)
#         super()
#         
#         setWindowTitle "Logger"
# 
#         setToolTip "This is a logger for my quadricopter"
#         
#   logWidget =  Qt::TextEdit.new('Hello World!', self)
#   logWidget.ensureCursorVisible
#   Thread.new {
#     while true 
#       begin
#         line = sp.readline
#     rescue
#       line = "/!\ error while reading serialport"
#     end
#       logWidget.append(line)
#       #logWidget.moveCursor (Qt::TextCursor) 
#   end
#     }
#         resize 800, 600
#         move 300, 300
# 
#         show
#     end
# end

# app = Qt::Application.new ARGV

#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_VERBOSE 5


def gnuplot(file, commands)
  IO.popen("gnuplot #{file} 2>/dev/null", "w") { |io| io.puts commands }
end



logLevels = Hash.new
logLevels[0] = Hash.new
logLevels[0][:name] = 'No Output'
logLevels[0][:color] = reset

logLevels[1] = Hash.new
logLevels[1][:name] = 'Error'
logLevels[1][:color] = red

logLevels[2] = Hash.new
logLevels[2][:name] = 'Warning'
logLevels[2][:color] = yellow

logLevels[3] = Hash.new
logLevels[3][:name] = 'Info'
logLevels[3][:color] = blue

logLevels[4] = Hash.new
logLevels[4][:name] = 'Debug'
logLevels[4][:color] = cyan

logLevels[5] = Hash.new
logLevels[5][:name] = 'Verbose'
logLevels[5][:color] = green

modules = []
modules[0] = 'Main'
modules[1] = 'UNKNOW'

signalsLogs = Hash.new

def myReadLine sp
  buffer = ""
  pchar = ""
  while true
    char = sp.getc
    addChar two pchar.to_s
    addChar three pchar.unpack('c*')[0].to_s(16) << " "
    if(char == nil)
      next
    end
    
        buffer << char
  
    if( char == "\n" && pchar == "\r")
      return buffer
    end
    
    pchar = char
    
  end
end

  
require "ncurses"



def addLine(win, text)
    win.move( win.getcury(), 2 )
    win.addstr(text)
    win.box(?|, ?-)
end
    
def addChar(win, char)
    win.move( win.getcury()+3, win.getcurx() )    
    win.addstr(char)
    win.move( win.getcury()-3, win.getcurx() )
    if win.getcurx() >= Ncurses.COLS()/2
       win.move( win.getcury()+1, 1 )
    end
    win.box(?|, ?-)
end 

def drawLayout()
  # make a new window as tall as the screen and half as wide, in the left half
  # of the screen
  one = Ncurses::WINDOW.new(Ncurses.LINES()/2, Ncurses.COLS(),
                            0, 0)
  # make one for the right half
  two = Ncurses::WINDOW.new(Ncurses.LINES() - Ncurses.LINES()/2, Ncurses.COLS()/2,
                            Ncurses.LINES()/2, 0)
                            
  three = Ncurses::WINDOW.new(Ncurses.LINES() - Ncurses.LINES()/2, Ncurses.COLS()/2,
                            Ncurses.LINES()/2, Ncurses.COLS()/2)
                            
  #one.box("|", "-")
  #two.box('|', '?')
  #three.box('|', '-')
  one.scrollok(true)
  two.scrollok(true)
  three.scrollok(true)
  Thread.new{
    one.noutrefresh() # copy window to virtual screen, don't update real screen
     two.noutrefresh()
     three.noutrefresh()
     Ncurses.doupdate() # update read screen
     sleep(0.100)
  }
  
  
  
 
end
  

 begin
  # initialize ncurses
  Ncurses.initscr
  Ncurses.cbreak           # provide unbuffered input
  Ncurses.noecho           # turn off input echoing
  Ncurses.nonl             # turn off newline translation
  Ncurses.stdscr.intrflush(false) # turn off flush-on-interrupt
  Ncurses.stdscr.keypad(true)     # turn on keypad mode

  drawLayout
  port_str = "/dev/ttyACM" #may be different for you
baud_rate = 115200
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE
sp = nil
port = ''
(0..6).each{|portNumber|
  begin
    port = "#{port_str}#{portNumber}"
    sp = SerialPort.new(port, baud_rate, data_bits, stop_bits, parity)
    break
  rescue => e
    puts "#{port} not available"
  end


}
if(sp == nil)
  abort "serialPort not found"
end

puts "#{port} will be used"

#starting login
sp.write('U')
Thread.new{
  c = one.getch()
  addChar one c
  sp.write c
  }


f = File.open('main.log', 'w')
while true
  begin
#       line = sp.readline
    line = myReadLine sp
      res = line.scan(/{(\d+),\s(\d+),\s(\d+),\s(\d+)}/)
      if(res != [])
  logLevel = res[0][0].to_i
  moduleId = res[0][1].to_i
  signalId = res[0][2].to_i
#   time = res[0][3].to_i
  value = res[0][3].to_i
  fileName = "analog_#{moduleId}_#{signalId}.csv"
  
  if( signalsLogs[fileName] == nil )
    signalsLogs[fileName] = File.open(fileName, 'w')
          commands = "
      plot \"#{fileName}\" w l;
      pause 1
      replot
      reread
    "
    gp = File.open(".#{fileName}.gp", 'w')
    gp.write(commands)
    gp.close
    
      Thread.new {
       gnuplot(".#{fileName}.gp", '')
     }
  end
  
  signalsLogs[fileName].write("#{value},\n")
  
  signalsLogs[fileName].flush

  
      else
	addChar one "ooo"
  res = line.scan(/(\d+),\s(\d+),(.*)/)
  if(res == [])
    puts "INVALID TRAME => #{line}"
  else
    logLevel = res[0][0].to_i
    moduleId = res[0][1].to_i
    message = res[0][2]
     addLine one "#{logLevels[logLevel][:color]}#{logLevels[logLevel][:name]}  #{modules[moduleId]}  ===>  #{message}#{reset}"
  end
  f.write(line)
      end
  rescue
     
  end
  
    
    
end

ensure
  Ncurses.echo
  Ncurses.nocbreak
  Ncurses.nl
  Ncurses.endwin
end



#sp = false
# QtLogger.new sp
# app.exec








