async foo(some_args) {
  //Do stuff
  return promise(some_return_type);
}

int main(void)
{
  same_return_as_in_async_func result = await(foo);
  return EXIT_SUCCESS;
}
