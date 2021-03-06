# ospl_create_library
(since version 0.1.0)  
create an ospl library



## Description

```c
int ospl_create_library(char *library);
```
This function creates a ospl library at the given path. To see how an ospl library is structured, see [`ospl_structure`](/{{ site.baseurl }}/reference#structure)


## Parameters
**`library`**: the path where the library will be created.


## Return values

| `value`      | `signification`                              |
| ------------ | -------------------------------------------- |
| `SUCCESS`    | library successfully created                 |
| `ERR_EXISTS` | the path given already exists                |
| `ERR_DB`     | an error occurred when accessing to database |

## Example

```c
#include <ospl.h>

int main(void)
{
	int ret;
	char *library = "path/to/photo/library/";

	if ((ret = ospl_create_library(library)) < 0)
	{
		printf("Failed to create library: %s", ospl_enum_error(ret));
		return 1;
	}
	else
		printf("library successfully created");
	return 0;
}
```


## Behavior

This function creates a library by first creating the folder named after the library. If it succeeds, it will create the photos folder, then the import folder, and finally, the thumbnails folder. If every folder creation succeeds, the database file is created. If any of these operation fails, it will try to delete the folder.
Learn more at [`ospl db structure`](/{{ site.baseurl }}/reference#folder-structure)


## Changelog

| `version` | `description`                                                |
| --------- | ------------------------------------------------------------ |
| `v0.1.1`  | adapting to the new error return system & if any operation fails, it deletes the library folder |
| `v0.1.0`  | the function is introduced                                   |
