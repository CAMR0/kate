# -*- coding: utf-8 -*-
# Copyright (c) 2013 by Pablo Martín <goinnn@gmail.com>
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.

# This code is based in this other:
# <http://www.muhuk.com/2008/11/extending-kate-with-pate/>
# This file originally was in this repository:
# <https://github.com/goinnn/Kate-plugins/blob/master/kate_plugins/jste_plugins/json_plugins.py>

import sys
try:
    import simplejson as json
    LIB_JSON = 'simplejson'
except ImportError:
    import json
    LIB_JSON = 'standard'

from PyKDE4.kdecore import i18nc

from libkatepate import text
from libkatepate.errors import showError
from js_settings import (SETTING_INDENT_JSON,
                         SETTING_ENCODING_JSON)

PRETTIFY_JSON_SHORTCUT = 'Ctrl+Alt+J'


def prettify_JSON(view):
    """A simple JSON pretty printer. JSON formatter which a good indents"""
    source = view.selectionText()
    if not source:
        showError(i18nc('@info:whatsthis', 'Please select JSON text and press: <shortcut>%1</shortcut>', PRETTIFY_JSON_SHORTCUT))
    else:
        indent = SETTING_INDENT_JSON.lookup()
        encoding = SETTING_ENCODING_JSON.lookup()
        try:
            if LIB_JSON == 'simplejson' or sys.version_info.major == 2:
                target = json.dumps(json.loads(source),
                                    indent=indent,
                                    encoding=encoding)
            else:
                target = json.dumps(json.loads(source),
                                    indent=indent)
            view.removeSelectionText()
            text.insertText(target)
        except ValueError as e:
            showError(i18n('This selected text is not valid JSON: %1', e.message))

# kate: space-indent on; indent-width 4;
