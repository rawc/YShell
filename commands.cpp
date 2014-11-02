// $Id: commands.cpp,v 1.4 2014-07-07 18:08:30-07 - - $

/*
 *
 * This assignment was completed doing paired programming by
 * PARTNER: Louis Chatta (lchatta@ucsc.edu)
 * PARTNER: Patrick Mathieu (pmathieu@ucsc.edu)
 *
 */

#include "commands.h"
#include "debug.h"

commands::commands(): map ({
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
}){}

command_fn commands::at (const string& cmd) {
   command_map::const_iterator result = map.find (cmd);
   if (result == map.end()) {
      throw yshell_exn (cmd + ": no such function");
   }
   return result->second;
}


void fn_cat (inode_state& state, const wordvec& words){
   if(words.size()<2){
     throw yshell_exn("No specified file Name");
    }else{
     for(unsigned int i =1; i<words.size();i++){
       inode_ptr selected_file =
        state.get_cwd_node()->to_get_dir(words.at(i));
       if(selected_file->get_type() != 0)
         throw yshell_exn(words.at(0)+": "+words.at(1)+
         ": "+ "is not a file");
         wordvec data =  selected_file->to_get_file();
      for(vector<string>::iterator it = data.begin()+2;
         it!= data.end(); it++){
         cout << *it<< " ";
         }cout<<endl;
    }
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words){
  if(words.size()==1 or words.at(1).compare("/") ==0) {
     state.update_cwd(state.get_root()); 
     state.reset_path();
     return;
    }
  
      
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   string new_dir = words.at(1);
   inode_ptr selected_dir =
       state.get_cwd_node()->to_get_dir(new_dir);
    if(selected_dir->get_type() != 1)
       throw yshell_exn(words.at(0)+": "+words.at(1)+
      ": "+ "is not a directory");
   state.update_cwd(selected_dir);

   if(words[1] != ".") {
     if(words[1] == "..") {
       if(state.get_path_size() > 1) {
         state.pop_from_path();
       }
     } else {
       state.push_to_path(new_dir);
     }
   }
}

void fn_echo (inode_state& state, const wordvec& words){
  if(words.size()<2)
    throw yshell_exn("No specified string");  
   int lineNum {0};
   for (auto c : words){
     if(c=="echo"){

        continue;
       }
      cout << c << ' ';
      lineNum++;
      }
       cout <<"\n";
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size()>1){
    
    int exit_code = stoi(words.at(1));
    exit_status::set(exit_code);
    throw ysh_exit_exn();
    }else{
     throw ysh_exit_exn();
    }
}

void fn_ls (inode_state& state, const wordvec& words){
    
 
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() < 2) {
     state.get_cwd_node()->print_contents();
   } else {
     vector<string> _path {};
     inode_ptr i {nullptr};
     string s = words[1];
     string delimiter = "/";
     size_t pos = 0;
     string token;
     string curr_dir {};

     while((pos = s.find(delimiter)) != string::npos) {
       token = s.substr(0, pos);
       _path.push_back(token);
       s.erase(0, pos + delimiter.length());
     }
     //could use .split()
     _path.push_back(s);

     for(vector<string>::iterator it = _path.begin()
          ; it != _path.end(); it++) {
       if(*it == "") {
        if(it > _path.begin()) {
          continue;
         } else {
           i = state.get_root();
         }
       } else {
       i = state.get_cwd_node()->to_get_dir(*it);
       }
     }

     if(words.at(1) == "/") {
       cout << words.at(1) << ":" << endl;
     } else {
       cout << "/" << words.at(1) << ":" << endl;
     }
     i->print_contents();
   }
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   vector<string> _path = state.get_path();

   if(words.size() < 2 or words[1] == "/") {
     vector<string> _path {"/"};
     cout << "/:" << endl;
     state.get_root()->recursive_print_contents(_path);
     return;
   }

   for(vector<string>::iterator it = _path.begin()
       ; it != _path.end(); it++) {
     if(*it == _path[0] or it == _path.end() - 1) {
       cout << *it;
     } else {
       cout << *it;
     }
   }
   cout << ":" << endl;
   state.get_cwd_node()->recursive_print_contents(_path);
}


void fn_make (inode_state& state, const wordvec& words){
  if(words.size()<2){ 
    throw yshell_exn("No specified file name");
   }else{
    if(words.at(1).find("/") != string::npos){
      throw yshell_exn(words.at(0)+ ": "+ 
     words.at(1)+": "+ "Cannot accept directories");
    }
   }
  state.add_file(words);

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
  
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   inode_ptr child {};
   inode_ptr parent = state.get_cwd_node();

   if(words.size() < 2) {
     throw yshell_exn("No directory name specified");
   } else if(words.size() > 3) {
     throw yshell_exn("Too many arguments");
   } else {
     child = parent->to_add_dir(words[1]);
   }
   
   child->to_init_dir(child, parent);
}
void fn_prompt (inode_state& state, const wordvec& words){
   string err{}; 
   err +=  words.at(0) + ": "
     + "No specified file name";

 if(words.size()<2) 
    throw yshell_exn(err); 
    
     state.do_prompt(words);
  
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_pwd (inode_state& state, const wordvec& words){

   state.print_path();

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   inode_ptr i = state.get_cwd_node()->to_get_dir(words.at(1));
   if(i->get_type() == DIR_INODE) {
     throw yshell_exn("Specified argument is a directory. Try rmr.");
   }

   if(words.size()<2){ 
     throw yshell_exn("No specified file/folder name"); 
    }
  
   if(words.at(1) == "."){ 
     throw yshell_exn("rm: . : can not be removed"); 
   }
   if(words.at(1)== ".."){
      throw yshell_exn("rm: .. : can not be removed"); 
   }

   state.get_cwd_node()->to_rm_dir(words[1]);
}

void fn_rmr (inode_state& state, const wordvec& words){
  if(words.size()<2) 
    throw yshell_exn("No specified file/folder name");

   
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words[1] == ".") {
     throw yshell_exn("Can't remove that directory");
   } else if(words[1] == "..") {
     throw yshell_exn("Can't remove that directory");
   } else if(words[1] == "/") {
     throw yshell_exn("Can't remove the root directory");
   }

   inode_ptr i = state.get_cwd_node()->to_get_dir(words[1]);
   i->recursive_rm_contents();
   state.get_cwd_node()->to_rm_dir(words[1]);
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

