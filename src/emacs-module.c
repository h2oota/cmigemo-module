/*
  Copyright (c) 2023 HIROSHI OOTA.
  All rights reserved.

  Redistribution and use in source and binary forms are permitted
  provided that the above copyright notice and this paragraph are
  duplicated in all such forms and that any documentation,
  advertising materials, and other materials related to such
  distribution and use acknowledge that the software was developed
  by the <organization>.  The name of the
  <organization> may not be used to endorse or promote products derived
  from this software without specific prior written permission.
  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <emacs-module.h>
#include <migemo.h>

#define CHECK_TYPE(e, v, t) ((e)->eq((e), (e)->intern((e), t), (e)->type_of((e), v)))
#define elementof(a) (sizeof(a) / sizeof(a[0]))
#define FUNCALL(e, n, a) (e)->funcall((e), (e)->intern((e), n), elementof(a), a)
#define MAKE_STRING(e, s) (e)->make_string((e), (s), strlen(s))
#define MAKE_UB_STRING(e, s) (e)->make_unibyte_string((e), (s), strlen(s))

static void cmigemo_finalizer(void *data)
{
     if (*(migemo **)data != NULL) {
	  migemo_close(*(migemo **)data);
	  *(migemo **)data = NULL;
     }
     free(data);
}

static migemo **extract_migemo(emacs_env *env, emacs_value v)
{
     emacs_value mgm = env->funcall(env, env->intern(env, "get"), 2, (emacs_value[]){
	       v, env->intern(env, "pmigemo"),});
     if (env->non_local_exit_check(env) != emacs_funcall_exit_return)
	  return NULL;

     if (env->get_user_finalizer(env, mgm) != cmigemo_finalizer) {
	  emacs_value a[] = {
	       MAKE_STRING(env, "cmigemo-user-ptrp %s"), v,
	  };
	  env->non_local_exit_signal(env, env->intern(env, "cmigemo-invalid-migemo-handle"),
				     FUNCALL(env, "format", a));
	  return NULL;
     }
     return (migemo **)env->get_user_ptr(env, mgm);
}

static const char cmigemo_open_doc[] =
    "Open a C/Migemo session with DICT-FILE and returns cmigemo session handle.\n"
    "The remaining PARAMETERS should be SUBDICTs or a sequence of :coding-system and DECODING.\n"
    "If a DECODING is given, the cmigemo-query decodes the result, returns it."
    "\n\n(fn DICT-FILE &rest PARAMETERS)";

static emacs_value cmigemo_open(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data)
{
     const emacs_value
	  Qnoerror = env->intern(env, ":no-error"),
	  Qcoding_system = env->intern(env, ":coding-system"),
	  Snil = env->intern(env, "nil"),
	  Sstatus = env->intern(env, "status"),
	  result = env->funcall(
	       env, env->intern(env, "gensym"), 0, NULL),
	  Fput = env->intern(env, "put");

     env->funcall(env, Fput, 3, (emacs_value[]){
	       result, Sstatus, env->intern(env, "closed"), });

     // check all arguemnts
     bool coding_system_found = false;
     for (int i = 0; i < nargs; i++) {
	  if (i > 0 && env->eq(env, args[i], Qnoerror))
	       continue;

	  // :coding-system
	  if (i > 0 && env->eq(env, args[i], Qcoding_system)) {
	       if (i == nargs - 1) {
		    env->non_local_exit_signal(env, env->intern(env, "coding-system-error"),
					       MAKE_STRING(env, "missing coding-system"));
		    return NULL;
	       }
	       if (coding_system_found) {
		    env->non_local_exit_signal(env, env->intern(env, "coding-system-error"),
					       MAKE_STRING(env, "coding-system already specified."));
		    return NULL;
	       }
	       if (!CHECK_TYPE(env, args[i+1], "symbol")) {
		    emacs_value a[] = {
			 MAKE_STRING(env, "symbolp %s"), args[i+1],
		    };
		    env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"),
					       FUNCALL(env, "format", a));
		    return NULL;
	       }
	       coding_system_found = true;
	       i += 1;
	       continue;
	  }

	  if (!CHECK_TYPE(env, args[i], "string")) {
	       emacs_value a[] = {
		    MAKE_STRING(env, "stringp %s"), args[i],
	       };
	       env->non_local_exit_signal(env, env->intern(env, "wrong-type-argument"),
					  FUNCALL(env, "format", a));
	       return NULL;
	  }
     }

     ptrdiff_t len = 0;

     env->copy_string_contents(env, args[0], NULL, &len);
     if (env->non_local_exit_check(env) != emacs_funcall_exit_return)
	  return NULL;

     char dict[len];
     env->copy_string_contents(env, args[0], dict, &len);
     migemo *pmigemo = migemo_open(dict);

     if (pmigemo == NULL) {
	  env->non_local_exit_signal(env, env->intern(env, "file-error"), args[0]);
	  return NULL;
     }

     bool skiperror = false;
     for (int i = 1; i < nargs; i++) {
	  if (env->eq(env, args[i], Qnoerror)) {
	       skiperror = true;
	       continue;
	  }
	  // coding-system
	  if (env->eq(env, args[i], Qcoding_system)) {
	       env->funcall(env, Fput, 3, (emacs_value[]){
			 result, env->intern(env, "coding-system"), args[i+1]});

	       i += 1;
	       continue;
	  }

	  ptrdiff_t len = 0;
	  env->copy_string_contents(env, args[i], NULL, &len);
	  if (env->non_local_exit_check(env) != emacs_funcall_exit_return) {
	       migemo_close(pmigemo);
	       return NULL;
	  }
	  char subdict[len];
	  env->copy_string_contents(env, args[i], subdict, &len);
	  if (migemo_load(pmigemo, MIGEMO_DICTID_MIGEMO, subdict) != MIGEMO_DICTID_MIGEMO) {
	       if (skiperror) {
		    emacs_value a[] = {
			 MAKE_STRING(env, "cmigemo subdict load error %s"), args[i],
		    };
		    FUNCALL(env, "message", a);
	       } else {
		    env->non_local_exit_signal(env, env->intern(env, "file-error"), args[i]);
		    migemo_close(pmigemo);
		    return NULL;
	       }
	  }
     }
     migemo_set_operator(pmigemo, MIGEMO_OPINDEX_OR, (const unsigned char *)"\\|");
     migemo_set_operator(pmigemo, MIGEMO_OPINDEX_NEST_IN, (const unsigned char *)"\\(");
     migemo_set_operator(pmigemo, MIGEMO_OPINDEX_NEST_OUT, (const unsigned char *)"\\)");
     migemo_set_operator(pmigemo, MIGEMO_OPINDEX_NEWLINE, (const unsigned char *)"\\s-*");

     migemo **p = malloc(sizeof(migemo *));
     if (p == NULL) {
	  migemo_close(pmigemo);
	  env->non_local_exit_signal(env, env->intern(env, "memory-error"), Snil);
	  return NULL;
     }
     *p = pmigemo;
     env->funcall(env, Fput, 3, (emacs_value[]){
	       result, env->intern(env, "pmigemo"),
	       env->make_user_ptr(env, cmigemo_finalizer, p)});
     env->funcall(env, Fput, 3, (emacs_value[]){
	       result, Sstatus, env->intern(env, "opened"), });
     return result;
}

static const char cmigemo_query_doc[] =
     "do query for ROMAN-STRING."
     "\n\n(fn CMIGEMO-SESSION-HANDLE ROMAN-STRING)";

static emacs_value cmigemo_query(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data)
{
     migemo **pmigemo = extract_migemo(env, args[0]);

     if (pmigemo == NULL)
	  return NULL;

     if (*pmigemo == NULL) {
	  env->non_local_exit_signal(
	       env, env->intern(env, "cmigemo-closed"), env->intern(env, "nil"));
	  return NULL;
     }

     ptrdiff_t len = 0;
     env->copy_string_contents(env, args[1], NULL, &len);
     if (env->non_local_exit_check(env) != emacs_funcall_exit_return)
	  return NULL;
     char word[len];
     env->copy_string_contents(env, args[1], word, &len);
     unsigned char *ans = migemo_query(*pmigemo, (const unsigned char *)word);

     if (ans == NULL)
	  return env->intern(env, "nil");

     emacs_value result = MAKE_UB_STRING(env, (char *)ans);
     migemo_release(*pmigemo, ans);

     emacs_value coding_system = env->funcall(
	  env, env->intern(env, "get"), 2, (emacs_value[]){
	       args[0], env->intern(env, "coding-system")});

     return env->eq(env, env->intern(env, "nil"), coding_system)
	  ? result
	  : env->funcall(env, env->intern(env, "decode-coding-string"),
			 2, (emacs_value[]){result, coding_system});
}

static const char cmigemo_close_doc[] =
     "Close CMIGEMO-SESSION-HANDLE"
     "\n\n(fn CMIGEMO-SESSION-HANDLE)";

static emacs_value cmigemo_close(emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data)
{
     migemo **pmigemo = extract_migemo(env, args[0]);

     if (pmigemo == NULL)
	  return NULL;
     if (*pmigemo)
	  migemo_close(*pmigemo);
     *pmigemo = NULL;

     env->funcall(env, env->intern(env, "put"), 3, (emacs_value[]){
	       args[0], env->intern(env, "status"),
	       env->intern(env, "closed"), });

     return env->intern(env, "nil");
}

static void initialize_module(emacs_env *env)
{
    emacs_value defalias = env->intern(env, "defalias");
    env->funcall(
	 env, defalias, 2,
	 (emacs_value[]){
	      env->intern(env, "migemo-cmigemo-open"),
	      env->make_function(env, 1, emacs_variadic_function,
				 cmigemo_open, cmigemo_open_doc, NULL)});
    env->funcall(
	 env, defalias, 2,
	 (emacs_value[]){
	      env->intern(env, "migemo-cmigemo-query"),
	      env->make_function(env, 2, 2, cmigemo_query, cmigemo_query_doc, NULL)});

    env->funcall(
	 env, defalias, 2,
	 (emacs_value[]){
	      env->intern(env, "migemo-cmigemo-close"),
	      env->make_function(env, 1, 1, cmigemo_close, cmigemo_close_doc, NULL)});

    env->funcall(
	 env, env->intern(env, "provide"), 1,
	 (emacs_value[]){
	      env->intern(env, "migemo-cmigemo-module")});
}

int plugin_is_GPL_compatible;
int emacs_module_init (struct emacs_runtime *ert)
{
  if (ert->size < sizeof (*ert))
    return 1;

  emacs_env *env = ert->get_environment(ert);
  if (env->size < sizeof (*env))
    return 2;

  initialize_module(env);
  return 0;
}

// Local Variables:
// coding: utf-8
// indent-tabs-mode: nil
// End:
