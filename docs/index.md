### What is libospl ?

libospl is a fast C library for easily managing a photolibrary. You can use libospl to manage and import your folders, albums, and photos. It will automatically generate a thumbnail and put the photos in the correct place. All information is stored in an sqlite3 database.

### Example 
```c
#include <ospl.h>

int main(void)
{
	int ret;
	char *library = "path/to/photo/library";
	if ((ret = ospl_create_library(library)) < 0)
	{
		printf("Failed to create library: %s", ospl_enum_error(ret));
		return 1;
	}
	if ((ret = ospl_import_photo(library, "path/to/photo.jpg")) < 0)
	{
		printf("Failed to import photo: %s", ospl_enum_error(ret));
		return 1;
	}
}
```

This will create a library at `path/to/photo/library` and import the photo located at `path/to/photo.jpg`
