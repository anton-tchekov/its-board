#include "level.h"

const char levels[] =
	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                  #"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                    12#"
	"#                                    34#"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                  #"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                    ##              12#"
	"#                    ##              34#"
	"#                ######         ##   ###"
	"#                ######         ##     #"
	"#       ##       ######         ##     #"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                    12#"
	"#                                    34#"
	"#  ab                              #####"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                               ##     #"
	"#                    ##         ##     #"
	"#                    ##         ##     #"
	"#                ######         ##     #"
	"#                ######         ##     #"
	"#       ##       ######         ##     #"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                       ###      #     #"
	"#                                #   12#"
	"#        #    ##                 #   34#"
	"#  ab  ###    ##              #  #######"
	"#  cd ##            ##                 #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                               ##     #"
	"#                    ##         ##     #"
	"#                    ##         ##     #"
	"#                ######         ##     #"
	"#                ######         ##     #"
	"#       ##       ######         ##     #"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                #     #"
	"#                                #   12#"
	"#                       ###      #   34#"
	"#  ab         ##                 #######"
	"#  cd ##                               #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                               ##     #"
	"#                    ##         ##     #"
	"#                    ##         ##     #"
	"#                ######         ##     #"
	"#                ######         ##     #"
	"#       ##       ######         ##     #"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                  #"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                    ##              12#"
	"#                    ##              34#"
	"#                ######         ##   ###"
	"#                ######         ##     #"
	"#    AA ##  AA   ######   AA    ## AA  #"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                  #"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                    ##              12#"
	"#                    ##              34#"
	"#                ######         ##   ###"
	"#                ######         ##     #"
	"#AAAAAAA##AAAAAAA######AAAAAAAAA## AAAA#"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                  #"
	"#  cd                                  #"
	"#  ##                                  #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                A     #"
	"#                    ##         ##   12#"
	"#                  AA##         ##   34#"
	"#                ######         ##   ###"
	"#                ######         ##     #"
	"#AAAAAAA##AAAAAAA######AAAAAAAAA## AAAA#"
	"########################################"

	"########################################"
	"#       #VVVVV  VV   #                 #"
	"#       #            #                 #"
	"#       #            #                 #"
	"#       #            #                 #"
	"#       #            #                 #"
	"#       #            #                 #"
	"#  ab   #   ####    ###                #"
	"#  cd   #      #    V#V                #"
	"#  ##   #      #     V                 #"
	"#       #      #                       #"
	"#       #      #                       #"
	"#       #      #                       #"
	"#       ###    #                 A     #"
	"#              #     ##         ##   12#"
	"#                  AA##         ##   34#"
	"#                ######         ##   ###"
	"#                ######         ##     #"
	"#AAAAAA#####AAAAA######AAAAAAAAA## AAAA#"
	"########################################"

	"########################################"
	"#       #VVVVV  VV   #          ##     #"
	"#       #            #           V     #"
	"#       #            #              12 #"
	"#       #            #              34 #"
	"#       #            #                 #"
	"#       #            #                 #"
	"#  ab   #   ####    ###                #"
	"#  cd   #      #    V#V          A     #"
	"#  ##   #      #     V         ###     #"
	"#       #      #                ##     #"
	"#       #      #                ##LLLLL#"
	"#       #      #                ##LLLLL#"
	"#       ###    #                ##LLLLL#"
	"#              #     ##         ##LLLLL#"
	"#                  AA##         ##LLLLL#"
	"#                ######         ##LLLLL#"
	"#                ######         ##LLLLL#"
	"#AAAAAA#####AAAAA######AAAAAAAAA##LLLLL#"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                 12   #"
	"#                 AA              34   #"
	"#  ab           H####HHHHH  HHHH####   #"
	"#  cd           H    H                 #"
	"#  ##           H    HAAAAAAAAAA       #"
	"#               H    H##########       #"
	"#               H    H                 #"
	"#               H    H                 #"
	"#               H    H                 #"
	"#               H    H                 #"
	"#               H    H                 #"
	"#               H    H                 #"
	"#               H    H                 #"
	"###########HHHHHHHHHHHHHHHHHHHHHHHH#####"
	"###########LLLLLLLLLLLLLLLLLLLLLLLL#####"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#                                      #"
	"#  ab                                12#"
	"#  cd                         A   AA 34#"
	"#  ##                         ##########"
	"#                             #        #"
	"#                           ###        #"
	"#               AA                     #"
	"#             ####                     #"
	"#             #  #                     #"
	"#            A#  #       ##            #"
	"#           ###  #       ##            #"
	"#           #    #       ##            #"
	"#           #    #       ##            #"
	"#           #    #       ##LLLLLLLLLLLL#"
	"#     ##AAAA#    #AAAAAAA##LLLLLLLLLLLL#"
	"########################################"

	"########################################"
	"#                                      #"
	"#                                      #"
	"#   #   #   ###   #   #                #"
	"#   #   #  #   #  #   #                #"
	"#    # #   #   #  #   #                #"
	"#     #    #   #  #   #                #"
	"#     #    #   #  #   #                #"
	"#     #     ###    ###                 #"
	"#                                      #"
	"#                                      #"
	"#            #     #  ###  #   #   #   #"
	"#            #     #   #   ##  #   #   #"
	"#            #  #  #   #   # # #   #   #"
	"#            #  #  #   #   # # #   #   #"
	"#            #  #  #   #   #  ##       #"
	"#             ## ##   ###  #   #   #   #"
	"#ab                                  12#"
	"#cd                                  34#"
	"########################################";

typedef char check_levels_size[
	(sizeof(levels) == LEVEL_COUNT * LEVEL_WIDTH * LEVEL_HEIGHT + 1) ? 1 : -1];