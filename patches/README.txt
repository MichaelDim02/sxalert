Patches
=======

In this directory you will find patches for sxalert that come in the form
of .diff files. These have been tested to work with specific versions of
sxalert, but may support other versions too.

To apply a patch, copy the diff file to the project's root directory
where the source code is and run:

	$ patch < file.diff

where file.diff is the diff file. 

Notes:
If a hunk fails, you'll have to add the patch manually from the .rej file.
You may also have to adjust your config file after applying a patch.
