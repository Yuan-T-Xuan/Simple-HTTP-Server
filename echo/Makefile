################################################################################
# Makefile                                                                     #
#                                                                              #
# Description: This file contains the make rules for echo.                     #
#                                                                              #
# Authors: Andy Wei <cw829@cornell.edu>,                                       #
#          Tony Xuan <yx424@cornell.edu>                                       #
#                                                                              #
################################################################################

default: echo_server echo_client

echo_server:
	@gcc echo_server.c -o echo_server -include logging.h logging.c

echo_client:
	@gcc echo_client.c -o echo_client

clean:
	@rm echo_server echo_client
