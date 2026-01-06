#ifndef CLASS_H
#define CLASS_H

#include <stddef.h>

typedef struct Class {
  size_t size;
  const char *name;
  void (*ctor)(void *self, void *args);
  void (*dtor)(void *self);
} Class;

typedef struct Object {
  const Class *class;
} Object;

void *new(const void class_vtable, void *args);
#endif // !CLASS_H
