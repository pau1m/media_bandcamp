Media: Bandcamp

This adds support for the Bandcamp audio video sharing service, available at http://www.bandcamp.com/

To use this module, you'll first need to install Embedded Audio Field, which is
packaged with Embedded Media Field (from http://drupal.org/project/emfield).

Set up a content type to use a Third Party Audio field as you normally would with emfield.
Also ensure that you have enabled the new Audio provider from the Admin screen at /admin/content/emfield.

Using this module

Add a CCK field type for emfield / third party audio and then configure default options at http://exp.dev/admin/content/emfield/emaudio. This CCK field will accept URLs from bandcamp and display the flash content given at that page. It will not accept direct pasting of embed codes.


