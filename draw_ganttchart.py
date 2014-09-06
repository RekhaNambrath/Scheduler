import pygame,sys,os
from pygame.locals import *
pygame.init()
#Initialising...
black=(0,0,0)
white=(255,255,255)
red=(255,0,0)
green=(0,255,0)
blue=(0,0,255)
size=(1500,600)
flag=0
#Setting up drawing coordinates
x=20
y=100
sch_y=110
lineno=1
#Creating the Screen
clock = pygame.time.Clock()
headfont=pygame.font.Font(None, 50)
writefont=pygame.font.Font(None, 22)
screen=pygame.display.set_mode(size)
pygame.display.set_caption("Ganttchart")
s = pygame.display.get_surface()
screen.fill(white)
#Extracting the file
if os.path.isfile(str(sys.argv[1])):
			infile=open(sys.argv[1])
title=(infile.readline()).strip()
writetext=headfont.render(title,2,[0,0,0])
screen.blit(writetext,[200,0])
for line in infile:
	if ':' in line and not ('time' in line):
			#Creating the GanttChart				
			if not 'Scheduler' in line:	
				processlist=line.split(":")			
				slot=int(processlist[2].strip())-int(processlist[0].strip())				
				p_x=6*(slot/2)
				p=pygame.draw.rect(screen,black,[x,y,6*slot,40],2)
				if int(processlist[1].strip())==1:				
					s.fill(Color("red"),p)
				elif int(processlist[1].strip())==2:				
					s.fill(Color("blue"),p)
				elif int(processlist[1].strip())==3:				
					s.fill(Color("green"),p)
				elif int(processlist[1].strip())==4:				
					s.fill(Color("yellow"),p)
				elif int(processlist[1].strip())==5:				
					s.fill(Color("brown"),p)
				elif int(processlist[1].strip())==0:				
					s.fill(Color("violet"),p)
				p=pygame.draw.rect(screen,black,[x,y,6*slot,40],2)
				writetext=writefont.render(processlist[0],2,[0,0,0])
				screen.blit(writetext,[x,y-20])
				writetext=writefont.render(processlist[1],2,[0,0,0])
				screen.blit(writetext,[x+p_x,sch_y])
				writetext=writefont.render(processlist[2],2,[0,0,0])
				screen.blit(writetext,[x+6*slot,y-20])					
												
			if 'Scheduler' in line: 
				flag=1									
				processlist=line.split(":")
				slot=int(processlist[2].strip())-int(processlist[0].strip())				
				p=pygame.draw.rect(screen,black,[x,y,6*slot,40],2)
				s.fill(Color("black"),p)
				p=pygame.draw.rect(screen,black,[x,y,6*slot,40],2)					
				writetext=writefont.render(str(slot),2,[255,255,255])
				screen.blit(writetext,[x+5,sch_y])	
				pygame.display.flip()					
			if x>=1100 and lineno==1:
				x=20
				y=200
				sch_y=210
				lineno+=1
			elif x>=1100 and lineno==2:
				x=20
				y=300
				sch_y=310
				lineno+=1
			elif x>=1100 and lineno==3:
				x=20
				y=400
				sch_y=410
				lineno+=1
			else:
				x+=6*slot		
while True:	
	for event in pygame.event.get():
		if event.type==pygame.QUIT:		
			sys.exit()
	clock.tick(20)	
	pygame.display.update()
