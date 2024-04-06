void write_file_names(char* path);
int is_dir(char* path);
void modification_time(struct tm *mod_time);
void just_ls(char* argv[], int start_path, int argc);
void  access_rights(struct stat* p);
void l_file_info(char* full_path, char* file_name, int g);
void ls_l_g(char *path, int g);
void ls_R(char *path, int l, int g);
int ls(int argc, char* argv[]);