# Translation Guide

#### Run the following steps to add a new language translation for the nettoe project

* Clone the repository:

`git clone https://github.com/LaimeJesus/nettoe`

* In the terminal run:

`./configure`
`autoreconf -i`
`make`
    
* In dir /po:

`msginit -l es_AR`

Where `es` is the language translation abbreviation and `AR` is the country abbreviation.
This command will create the next file: `es_AR.po` in dir `/po`

* In the new file `es_AR.po` add the translations, for example: 

``` 
#: src/nettoe.c:220
#, c-format
msgid "Main Menu"
msgstr "Menu Principal"
```

* In file po/LINGUAS add

`es_AR`

* In terminal, update the translations.

`make update-po` 

* In terminal, install nettoe with translations

`make`
`make install`

### HOW TO USE:

By default the nettoe game use the system language. You can verify it using `locale` command.

If you want change, run:
 `LANGUAGE=es_PO nettoe`
 
### Common problems

if your language need special characters like accents, you will need to set up the content type of the `po/nettoe.pot` file to

```
"Content-Type: text/plain; charset=UTF-8\n"
```