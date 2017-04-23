##############################################################################
################################# makefile ###################################
##############################################################################
#									     #
#   makefile of MSArbor 						     #
#									     #
#   'make clean' cleans up						     #
#   'make' builds the module						     #
#									     #
#                                VERSION 1.00				     #
#                	        06 - 10 - 2004				     #
#									     #
#			      Antonio Frangioni				     #
#   			   Operations Research Group			     #
#			  Dipartimento di Informatica			     #
#   			     Universita' di Pisa			     #
#									     #
##############################################################################

# module name
NAME = MSArbor

# debug switches
SW = -g

# production switches
# SW = -O3

# libreries
LIB = -lm

# compiler
CC = g++

# basic directory
DIR = ./

# default target- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

default: $(NAME)

# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f $(DIR)*.o $(DIR)*~ $(NAME)

# main module (linking phase) - - - - - - - - - - - - - - - - - - - - - - - -

$(NAME): $(DIR)MSArbor.o $(DIR)Main.o
	$(CC) -o $(NAME) $(DIR)Main.o $(DIR)MSArbor.o $(LIB) $(SW)

# dependencies: every .o from its .C + every recursively included .h- - - - -

$(DIR)MSArbor.o: $(DIR)MSArbor.C $(DIR)MSArbor.h
	$(CC) -c $*.C -o $@ $(SW)

$(DIR)Main.o: $(DIR)Main.C $(DIR)MSArbor.h
	$(CC) -c $*.C -o $@ $(SW)

############################ End of makefile #################################
