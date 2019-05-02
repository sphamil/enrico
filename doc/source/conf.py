# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import os
import subprocess
import re

# -- Project information -----------------------------------------------------

project = 'ENRICO'
copyright = '2019, UChicago Argonne, LLC'
author = 'ENRICO Development Team'

# The full version, including alpha/beta/rc tags
release = '0.1'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'breathe',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'bootstrap-astropy'
html_theme_options = {'logotext1': 'ENRICO', 'logotext2': '', 'logotext3': ''}
html_show_sphinx = False

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# -- Breathe configuration ---------------------------------------------------

breathe_projects = {"enrico": "doxygen/xml"}
breathe_default_project = "enrico"


# -- Build Doxygen ---------------------------------------------------

def build_doxygen(app):
    # XML goes in Sphinx source dir, and HTML goes in Sphinx output dir

    doxygen_xmldir = os.path.abspath(os.path.join(app.srcdir, 'doxygen', 'xml'))
    doxygen_htmldir = os.path.abspath(os.path.join(app.outdir, 'doxygen', 'html'))

    # Doxygen won't create *nested* output dirs, so we do it ourselves.

    for d in (doxygen_xmldir, doxygen_htmldir):
        os.makedirs(d, exist_ok=True)

    # To pass output dirs to Doxygen, we follow this advice:
    # http://www.doxygen.nl/manual/faq.html#faq_cmdline
    # Here we read the Doxyfile into a string, replace the *_OUTPUT vars, and pass the string as
    # stdin to the doxygen subprocess

    with open('../Doxyfile') as f:
        doxy_opts = f.read()
    doxy_opts = re.sub(r'(\bHTML_OUTPUT\b\s*=\s*).*', r'\1"{}"'.format(doxygen_htmldir),
                       doxy_opts)
    doxy_opts = re.sub(r'(\bXML_OUTPUT\b\s*=\s*).*', r'\1"{}"'.format(doxygen_xmldir), doxy_opts)
    subprocess.run(['doxygen', '-'], cwd='..', input=doxy_opts, universal_newlines=True, check=True)


# -- Setup hooks -------------------------------------------------------------

def setup(app):
    app.add_stylesheet('theme_overrides.css')
    app.connect("builder-inited", build_doxygen)
