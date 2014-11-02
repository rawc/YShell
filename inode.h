// $Id: inode.h,v 1.3 2014-07-07 18:08:30-07 - - $
/*
 *
 * This assignment was completed doing paired programming by
 * PARTNER: Louis Chatta (lchatta@ucsc.edu)
 * PARTNER: Patrick Mathieu (pmathieu@ucsc.edu)
 *
 */


#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
using namespace std;

#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {PLAIN_INODE, DIR_INODE};
//enum creates a new type of variable
//the limitations are the specified args
//can be either PLAIN_INODE or DIR_INODE
class inode;
class file_base;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using file_base_ptr = shared_ptr<file_base>;
using plain_file_ptr = shared_ptr<plain_file>;
using directory_ptr = shared_ptr<directory>;

//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//

class inode_state {
   friend class inode;
   
   friend ostream& operator<< (ostream& out, const inode_state&);
   private:
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};
      string prompt {"% "};
      vector <string> dir_names;
      vector<string> path;
      //
   public:
     inode_state();
     string get_dir_last();
     //will return our last specified dir
     vector<string> get_dir();
     //will return our entire data struc
      
     string get_prompt();
     //returns prompt
     inode_ptr get_root();
     inode_ptr get_cwd_node();
     void do_prompt (const wordvec& words);
     void do_add (string new_path);
     void add_dir(string dir_name);
     void remove_dir(string dir_name);
     void change_root(inode_ptr new_root);
     void change_root_node(inode& root);
     void change_cwd_node(inode& cwd);
     void add_file(const wordvec& words);    
     void printPath();
     //prints path as a string
   

     void update_root(inode_ptr new_root);
     void update_cwd(inode_ptr new_cwd);
     void push_to_path(string dir_name);
     void pop_from_path();
     vector<string> get_path();
     void print_path();
     size_t get_path_size();
     void reset_path();
};


//
// class inode -
//
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//    

class inode {
   friend class inode_state;
   private:
      static int next_inode_nr;
      int inode_nr;
      inode_t type;
      file_base_ptr contents;
   public:
      inode (inode_t init_type);
      int get_inode_nr() const;
      wordvec get_file_contents();
      map<string, inode_ptr>  get_contents();
      void push_contents(map<string,inode_ptr> mapl);
      void do_add_words();
      void do_add_map(string key, inode_ptr inod);
      void do_print_map();
      void print_contents();       
      // Extends to read_from_map
      void recursive_print_contents(vector<string> path);       
      void recursive_rm_contents();
      inode_ptr to_add_dir(string dir_name);       
      void to_rm_dir(string dir_name);       
      void to_init_dir(inode_ptr child, inode_ptr parent);
      inode_t get_type();
      inode_ptr to_get_dir(string dir_name);
      wordvec to_get_file();
      void add_words(const wordvec& words);
      bool do_check(const string& dir_name);
      int get_size();
};

//
// class file_base -
//
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.
//

class file_base {
   protected:
      file_base () = default;
      file_base (const file_base&) = default;
      file_base (file_base&&) = default;
      file_base& operator= (const file_base&) = default;
      file_base& operator= (file_base&&) = default;
      virtual ~file_base () = default;
      virtual size_t size() const = 0;
   public:
      friend plain_file_ptr plain_file_ptr_of (file_base_ptr);
      friend directory_ptr directory_ptr_of (file_base_ptr);
};


//
// class plain_file -
//
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
//    Throws an yshell_exn for a directory.
// writefile -
//    Replaces the contents of a file with new contents.
//    Throws an yshell_exn for a directory.
//

class plain_file: public file_base {
   private:
      wordvec data;
   public:
      size_t file_size;
      wordvec get_data();
      void push_data(const wordvec& words);
      size_t size() const override;
      const wordvec& readfile() const;
      void writefile (const wordvec& newdata);
};

//
// class directory -
//
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an yshell_exn if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//  Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public file_base {
//does not need change
   private:
      map<string,inode_ptr> dirents;
   public:
      size_t dir_size;
      void do_size();
      void put_map(map<string,inode_ptr>dir);
      map<string,inode_ptr> get_map();
      void update_size();
      size_t size() const override;
      void remove (const string& filename);
      inode& mkfile (const string& filename);
      void read_from_map();
      void recursive_read_from_map(vector<string> path);
      void insert_to_map(string dir_name, inode_ptr dir_ptr);
      void rm_from_map(string dir_name);
      void recursive_rm_from_map();
      inode_ptr add_dir(string dir_name);
      void rm_dir(string dir_name);
      void init_dir(inode_ptr child, inode_ptr parent);
      inode_ptr get_dir(string dir_name);
      bool check_dir(string dir_name);
};

#endif
