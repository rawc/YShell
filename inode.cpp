// $Id: inode.cpp,v 1.4 2014-07-07 18:08:30-07 - - $
/*
 *
 * This assignment was completed doing paired programming by
 * PARTNER: Louis Chatta (lchatta@ucsc.edu)
 * PARTNER: Patrick Mathieu (pmathieu@ucsc.edu)
 *
 */


#include <cassert>
#include <iostream>
#include <fstream>
using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   //this is the inode constructer
   //inode
   inode_nr (next_inode_nr++), type (init_type)
   //this gives each new inode a new number
{
   switch (type) {
      case PLAIN_INODE:
           contents = make_shared<plain_file>();
           //here contetnts is created
           break;
      case DIR_INODE:
           contents = make_shared<directory>();
          //insert . and .. into directory name
          //make inode can also hold
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

plain_file_ptr plain_file_ptr_of (file_base_ptr ptr) {
   plain_file_ptr pfptr = dynamic_pointer_cast<plain_file> (ptr);
   assert (pfptr != nullptr);
   return pfptr;
}

directory_ptr directory_ptr_of (file_base_ptr ptr) {
   directory_ptr dirptr = dynamic_pointer_cast<directory> (ptr);
   assert (dirptr != nullptr);
   return dirptr;
}


size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   //to remove check if there is a . or .. in directory
   //rmdir. rmdir.. or rmdir / are INVALID
}

void inode_state::do_prompt(const wordvec& words){
  prompt ="";
  for(const auto& c: words){ 
   if(c.compare("prompt")==0) continue;
    prompt += c;
    prompt +=" ";
}
}

string inode_state::get_prompt(){
  return prompt;

}

void inode_state::add_dir(string dir_name){
  dir_names.push_back(dir_name);
}

inode_ptr inode_state::get_root(){
  return root;


}

inode_ptr inode_state::get_cwd_node(){
   return cwd;
}

void inode_state::printPath(){
   int i=0;
   for(auto c: dir_names){
     if (i!=0) cout<<"/";
     if((i = 0)) cout<<" ";  
     cout<< c;
     i++;
     }cout<<endl; 



}

string inode_state::get_dir_last(){
    return dir_names.back();
   
}

vector <string> inode_state::get_dir(){
   return dir_names;

}

void inode::do_add_words(){
   wordvec word_vec = get_file_contents();
}

void inode::push_contents(map<string,inode_ptr>mapl){
  directory_ptr dir_ptr = directory_ptr_of(contents);
  dir_ptr->put_map(mapl); 
}

wordvec inode::to_get_file(){
   plain_file_ptr file =
     plain_file_ptr_of(contents);
  wordvec data = file->get_data();
  return data;
}

void directory::put_map(map<string,inode_ptr>dir){
   dirents = dir;
}

void inode::do_add_map(string key, inode_ptr inod ){
  map<string, inode_ptr> d = get_contents();
  d.insert(pair<string,inode_ptr>(key,inod));
  map<string, inode_ptr>::iterator it;
  push_contents(d); 
  it = d.begin();
}

void inode::do_print_map(){
  map<string, inode_ptr> d = get_contents();
  for(const auto& entry: d){
   cout<<"key: "<<entry.first<< "value: "<<entry.second<<endl;
  }
}

void inode_state::change_root_node(inode&  root){
    root = root;
    cout << root.contents<<endl;
}

map<string,inode_ptr> directory::get_map(){
     return dirents;
}

wordvec inode::get_file_contents(){
   plain_file_ptr plain_ptr = plain_file_ptr_of(contents);
   wordvec words = plain_ptr->get_data();
   return words;
}

wordvec plain_file::get_data(){
   return data;
}

map<string, inode_ptr> inode::get_contents(){
   directory_ptr dir_ptr = directory_ptr_of(contents);
   map <string, inode_ptr> dirents =
     dir_ptr->get_map(); 
   return dirents ;
}

void inode_state::change_cwd_node(inode& cwd){
    cwd = cwd;
}

void inode_state::do_add(string new_path){
    dir_names.push_back(new_path);
}

void inode_state::add_file(const wordvec& words){
   inode file_nod(PLAIN_INODE);
   string cmd_name = words.at(0);
   string file_name = words.at(1);
   bool t =cwd->do_check(file_name);
   if(t == false){
     file_nod.add_words(words);
   }else{
      throw yshell_exn(
       cmd_name + ":" + file_name+": "+"Already exists");
    }
   cwd->do_add_map
     (words.at(1), make_shared<inode>(file_nod));

}

bool inode::do_check(const string& dir_name){
   directory_ptr dir_ptr = directory_ptr_of(contents);
   bool t =dir_ptr->check_dir(dir_name);
  return t;

}

void inode::add_words(const wordvec& words){
   plain_file_ptr file 
     = plain_file_ptr_of(contents);
   wordvec tmp =  file->get_data();
   string title = words.at(1);
   string cmd = words.at(0);
   int itor {0};
   tmp = words;
   file->push_data(tmp);
   for(const auto& c : words){
     file->file_size+=c.size();
     itor++;
   }
   file->file_size-=title.size();
   file->file_size-=cmd.size();
   file->file_size+=(itor-3);
}

void plain_file:: push_data(const wordvec& words){
     data = words;
}

void inode_state::change_root(inode_ptr new_root){
    root = new_root;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt << "\"");

   inode root(DIR_INODE);
   update_root(make_shared<inode>(root));
   update_cwd(make_shared<inode>(root));
   directory_ptr dp = directory_ptr_of(root.contents);
   dp->insert_to_map(".", make_shared<inode>(root));
   dp->insert_to_map("..", make_shared<inode>(root));
   push_to_path("/");
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

void inode_state::update_root(inode_ptr new_root) {
   root = new_root;
}

void inode_state::update_cwd(inode_ptr new_cwd) {
   cwd = new_cwd;
}

void inode_state::push_to_path(string dir_name) {
   path.push_back(dir_name);
}

void inode_state::pop_from_path() {
   path.pop_back();
}

void inode_state::reset_path(){
   path.clear();
   path.push_back("/");
}

vector<string> inode_state::get_path() {
  return path;
}

void inode_state::print_path() {
   for(vector<string>::iterator it = path.begin();
     it != path.end(); it++) {
     if(*it == path[0] or it == path.end() - 1) {
       cout << *it;
     } else {
       cout << *it << "/";
     }
   }
   cout << endl;
}

size_t inode_state::get_path_size() {
   return path.size();
}

void inode::print_contents() {
   directory_ptr dp = directory_ptr_of(contents);
   dp->read_from_map();
}

void inode::recursive_print_contents(vector<string> path) {
   directory_ptr dp = directory_ptr_of(contents);
   dp->recursive_read_from_map(path);
}

void inode::recursive_rm_contents() {
  directory_ptr dp = directory_ptr_of(contents);
  dp->recursive_rm_from_map();
}

inode_ptr inode::to_add_dir(string dir_name) {
   directory_ptr dp = directory_ptr_of(contents);
   inode_ptr i_ptr = dp->add_dir(dir_name);
   return i_ptr;
}

void inode::to_init_dir(inode_ptr child, inode_ptr parent) {
   directory_ptr dp = directory_ptr_of(contents);
   dp->init_dir(child, parent);
}

inode_t inode::get_type() {
   return type;
}

inode_ptr inode::to_get_dir(string dir_name){
   directory_ptr dp = directory_ptr_of(contents);
   inode_ptr p = dp->get_dir(dir_name);
   return p;
}

int inode::get_size(){
  plain_file_ptr file{};
  directory_ptr dir {};
  if(type == 0){
    file = plain_file_ptr_of(contents);
    return file->file_size;
  }
  if (type == 1){
    dir = directory_ptr_of (contents);
    dir->do_size();
    return dir->dir_size;
  }

  return 0;
}

void inode::to_rm_dir(string dir_name) {
   directory_ptr dp = directory_ptr_of(contents);
   dp->rm_dir(dir_name);
}

void directory::do_size(){
   dir_size =0;
   map<string, inode_ptr>::iterator it;
   for(it = dirents.begin(); it != dirents.end(); it++) {

     dir_size++;
   }
}
 
void directory::read_from_map() {
   map<string, inode_ptr>::iterator it;
   for(it = dirents.begin(); it != dirents.end(); it++) {
     if(it->second->get_type() == DIR_INODE) {
       if(it->first == "." or it->first == "..") {
          cout<<"\t" << it->second->get_inode_nr() 
            << "\t" << it->second->get_size() 
            <<"\t" << it->first << endl;
       } else {
          cout << "\t" << it->second->get_inode_nr()
            << "\t" << it->second->get_size()
            << "\t" << it->first << "/" << endl;
       }
     } else {
       cout<<"\t" << it->second->get_inode_nr() 
            << "\t" << it->second->get_size() 
            <<"\t" << it->first << endl;
     }
   }
}

void directory::recursive_read_from_map
   (vector<string> path) {
   vector<pair<string, inode_ptr>> dirs {};
   for(map<string, inode_ptr>::iterator mapit = 
     dirents.begin(); mapit != dirents.end(); mapit++) {
     if((mapit->second)->get_type() == DIR_INODE) {
       if((mapit->first) != ".") {
         if((mapit->first) != "..") {
           dirs.push_back(pair<string, inode_ptr>
             (mapit->first, mapit->second));
         }
       }
     }
     if(mapit->second->get_type() == DIR_INODE) {
       if(mapit->first == "." or mapit->first == "..") {
         cout<<"\t" << mapit->second->get_inode_nr() 
            << "\t" << mapit->second->get_size() 
            <<"\t" << mapit->first << endl;
       } else {
         cout << "\t" << mapit->second->get_inode_nr()
            << "\t" << mapit->second->get_size()
            << "\t" << mapit->first << "/" << endl;
       }
     } else {
       cout<<"\t" << mapit->second->get_inode_nr() 
            << "\t" << mapit->second->get_size() 
            <<"\t" << mapit->first << endl;
     }
   }
   for(vector<pair<string, inode_ptr>>::iterator vecit = 
     dirs.begin(); vecit != dirs.end(); vecit++) {
     path.push_back(vecit->first);
     for(vector<string>::iterator vecit2 = 
       path.begin(); vecit2 != path.end(); vecit2++) {
       if(*vecit2 == path[0] or vecit2 == path.end() - 1) {
         cout << *vecit2;
       } else {
         cout << *vecit2 << "/";
       }
     }
     cout << ":" << endl;
     (vecit->second)->recursive_print_contents(path);
     path.pop_back();
   }
}

void directory::insert_to_map(string dir_name, inode_ptr dir_ptr) {
   dirents.insert(pair<string, inode_ptr>(dir_name, dir_ptr));
}

void directory::rm_from_map(string dir_name) {
   map<string, inode_ptr>::iterator it;
   it = dirents.find(dir_name);
   inode_ptr i = it->second;
   dirents.erase(it);
}

void directory::recursive_rm_from_map() {
  vector<pair<string, inode_ptr>> dirs {};
  for(map<string, inode_ptr>::iterator mapit =
    dirents.begin(); mapit != dirents.end(); mapit++) {
    if((mapit->second)->get_type() == DIR_INODE) {
      if((mapit->first) != ".") {
        if((mapit->first) != "..") {
          dirs.push_back(pair<string, inode_ptr>
            (mapit->first, mapit->second));
        }
      }
    }
  }
  for(vector<pair<string, inode_ptr>>::iterator vecit =
    dirs.begin(); vecit != dirs.end(); vecit++) {
    (vecit->second)->recursive_rm_contents();
  }
  for(map<string, inode_ptr>::iterator mapit2 =
    dirents.begin(); mapit2 != dirents.end(); mapit2++) {
    rm_from_map(mapit2->first);
  }
}

inode_ptr directory::add_dir(string dir_name) {
   if(dirents.find(dir_name) != dirents.end()) {
     throw yshell_exn(
       dir_name+ ": " + "Directory already exists");
   } else {
     inode i(DIR_INODE);
     inode_ptr i_ptr = make_shared<inode>(i);
     insert_to_map(dir_name, i_ptr);
     return i_ptr;
   }
}

void directory::init_dir(inode_ptr child, inode_ptr parent) {
   insert_to_map(".", child);
   insert_to_map("..", parent);
}

inode_ptr directory::get_dir(string dir_name){
   inode_ptr i{};

   // Possible error, come back to this
   if(dir_name.compare("./") == 0){
     dir_name=".";
    }else if(dir_name.compare("../") == 0){
       dir_name="..";
    }
   auto it = dirents.find(dir_name);
   if(it!=dirents.end()){
      i = it->second;
      return i;
    } else {
      throw yshell_exn(dir_name+": "+ "Not found");
    }
}

