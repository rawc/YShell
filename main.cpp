// $Id: main.cpp,v 1.1 2014-07-03 16:20:41-07 - - $
/*
 *
 * This assignment was completed doing paired programming by
 * PARTNER: Louis Chatta (lchatta@ucsc.edu)
 * PARTNER: Patrick Mathieu (pmathieu@ucsc.edu)
 *
 */


#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <unistd.h>

using namespace std;

#include "commands.h"
#include "debug.h"
#include "inode.h"
#include "util.h"

//
// scan_options
//    Options analysis:  The only option is -Dflags. 
//

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) option << ": invalid option"
                       << endl;
            break;
      }
   }
   if (optind < argc) {
      complain() << "operands not permitted" << endl;
   }
}


//
// main -
//    Main program which loops reading commands until end of file.
//

int main (int argc, char** argv) {
   execname (argv[0]);
   cout << boolalpha; // Print false or true instead of 0 or 1.
   cerr << boolalpha;
   cout << argv[0] << " build " << __DATE__ << " " << __TIME__ << endl;
   scan_options (argc, argv);
   inode_state state;
   bool need_echo = want_echo();
   commands cmdmap;
   string prompt = "%";
   
   state.add_dir("/  ");
   try {
      for (;;) {
         try {
   
            // Read a line, break at EOF, and echo print the prompt
            // if one is needed.
            prompt = state.get_prompt();
            cout << prompt << " ";
            string line;
            getline (cin, line);
            if (cin.eof()) {
               if (need_echo) cout << "^D";
               cout << endl;
               DEBUGF ('y', "EOF");
               break;
            }
            if (need_echo) cout << line << endl;
         
            // Split the line into words and lookup the appropriate
            // function.  Complain or call it.
            wordvec words = split (line, " \t");
            if(words.size()<1)continue;
            if(words.at(0)=="#") continue;
            //implementation of # comments
            //
            DEBUGF ('y', "words = " << words);
           
            command_fn fn = cmdmap.at(words.at(0));
            
            //here we make our function cmdmap 
            //and we locate our first input
            fn (state, words);
           //then we insert inode state & inode words into the map
         }catch (yshell_exn& exn) {
            // If there is a problem discovered in any function, an
            // exn is thrown and printed here.
            complain() << exn.what() << endl;
         }
      }
   } catch (ysh_exit_exn& ) {
      // This catch intentionally left blank.
   }

   return exit_status_message();
}
