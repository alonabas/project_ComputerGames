#################################################################
#
#				Warning!!!!
#				============
#	
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made 
# run it with "make clean all" source /cs/course/2013/cg/setpath.csh 
#
##################################################################

EXEC = 

#SHELL="/bin/sh"

# list of all the sources files
SRCS	=  	project.cpp		\
		Background.cpp	\
		bimage.cpp	\
		Camera.cpp	\
		FinalScene.cpp	\
		Firework.cpp	\
		Flag.cpp	\
		Flame.cpp	\
		Frame.cpp	\
		Globals.cpp	\
		Instructions.cpp	\
		MainModel.cpp	\
		MeshObject.cpp	\
		Mirror.cpp	\
		ModelFlags.cpp	\
		PPBObject.cpp	\
		RefractingObject.cpp	\
		ShaderIO.cpp	\
		Sky.cpp	\
		SkyBox.cpp	\
		Sound.cpp	\
		Statue.cpp	\
		SubScene.cpp	\
		Table.cpp	\
		Text.cpp	\
		TGALoader.cpp	\
		Torch.cpp
		

			
# compiler 
CC	= g++

#OpenMesh definitions
CG_HOME = /cs/course/2013/cg
OM_DIR = $(CG_HOME)/OpenMesh
OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
OM_INCLUDE= -I$(OM_DIR)/include

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/2013/cg/include $(OM_INCLUDE)

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
        -std=c++11  \
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated
					
						
else

	CFLAGS = 		\
        -std=c++11  \
		-Wall		\
		-DNDEBUG    \
		-O2
				 
endif


# linker
LD	= $(CC)
LDFLAGS	=

# the name of the lib to create 
APP = cgp_project

# each lib should add with -l flag
LIBS =  -L/usr/lib -lm -lopenal -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib $(OM_LIBS)


# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP)

$(OBJS) : $(SRCS)


tar:
	tar cvf $(EXEC).tar Makefile $(SRCS) $(HDRS) README
	

.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~ $(APP)