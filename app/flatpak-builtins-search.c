/* vi:set et sw=2 sts=2 cin cino=t0,f0,(0,{s,>2s,n-s,^-s,e-s:
 * Copyright © 2017 Patrick Griffis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *       Patrick Griffis <tingping@tingping.se>
 */

#include "config.h"

#include <glib/gi18n.h>

#include "flatpak-builtins.h"
#include "flatpak-builtins-utils.h"
#include "flatpak-dir-private.h"
#include "flatpak-table-printer.h"
#include "flatpak-utils-private.h"

static char *opt_arch;
static const char **opt_cols;
static gboolean opt_json;

static GOptionEntry options[] = {
  { "arch", 0, 0, G_OPTION_ARG_STRING, &opt_arch, N_("Arch to search for"), N_("ARCH") },
  { "columns", 0, 0, G_OPTION_ARG_STRING_ARRAY, &opt_cols, N_("What information to show"), N_("FIELD,…") },
  { "json", 'j', 0, G_OPTION_ARG_NONE, &opt_json, N_("Show output in JSON format"), NULL },
  { NULL}
};

static Column all_columns[] = {
  { "name",        N_("Name"),        N_("Show the name"),               1, FLATPAK_ELLIPSIZE_MODE_END, 1, 1 },
  { "description", N_("Description"), N_("Show the description"),        1, FLATPAK_ELLIPSIZE_MODE_END, 1, 1 },
  { "application", N_("Application ID"), N_("Show the application ID"),     1, FLATPAK_ELLIPSIZE_MODE_START, 1, 1 },
  { "version",     N_("Version"),     N_("Show the version"),            1, FLATPAK_ELLIPSIZE_MODE_NONE, 1, 1 },
  { "branch",      N_("Branch"),      N_("Show the application branch"), 1, FLATPAK_ELLIPSIZE_MODE_NONE, 1, 1 },
  { "remotes",     N_("Remotes"),     N_("Show the remotes"),            1, FLATPAK_ELLIPSIZE_MODE_NONE, 1, 1 },
  { NULL }
};


gboolean
flatpak_builtin_search (int argc, char **argv, GCancellable *cancellable, GError **error)
{
  g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
                       _("The search command is unsupported without AppStream support"));
  return FALSE;
}

gboolean
flatpak_complete_search (FlatpakCompletion *completion)
{
  g_autoptr(GOptionContext) context = NULL;

  context = g_option_context_new ("");
  if (!flatpak_option_context_parse (context, options, &completion->argc, &completion->argv,
                                     FLATPAK_BUILTIN_FLAG_ALL_DIRS | FLATPAK_BUILTIN_FLAG_OPTIONAL_REPO,
                                     NULL, NULL, NULL))
    return FALSE;

  flatpak_complete_options (completion, global_entries);
  flatpak_complete_options (completion, options);
  flatpak_complete_options (completion, user_entries);
  flatpak_complete_columns (completion, all_columns);
  return TRUE;
}
