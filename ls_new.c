char*
fmtname_new(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


void
ls_new(char *path, int cid)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int n, m;
  char id, filename[15];
  // int k;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls_new: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls_new: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    id = cid + '0';
    strcpy(filename, fmtname_new(buf));
    n = strlen(filename);
    m = 0;
    while(m < n){
      if(filename[m] == ' '){
        break;
      }
      else{
        m++;
      }
    }
    if( m < 2 || filename[m-2] != '@' || filename[m-1] == id){
      printf(1, "%s %d %d %d\n", filename, st.type, st.ino, st.size);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      printf(1, "ls_new: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls_new: cannot stat %s\n", buf);
        continue;
      }
      id = cid + '0';
      strcpy(filename, fmtname_new(buf));
      n = strlen(filename);
      m = 0;
      while(m < n){
        if(filename[m] == ' '){
          break;
        }
        else{
          m++;
        }
      }
      if( m < 2 || filename[m-2] != '@' || filename[m-1] == id){
        printf(1, "%s %d %d %d\n", filename, st.type, st.ino, st.size);
      }
    }
    break;
  }
  close(fd);
}
