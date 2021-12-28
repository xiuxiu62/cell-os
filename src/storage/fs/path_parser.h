#ifndef PATH_PARSER_H
#define PATH_PARSER_H

struct path_root {
  int drive_id;
  struct path_part *first;
};

struct path_part {
  const char *part;
  struct path_part *next;
};

#endif
