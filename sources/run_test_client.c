/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "cqlrt.h"
#include "run_test.h"
#include "alloca.h"

cql_code test_c_rowsets(sqlite3 *db);
cql_code test_rowset_same(sqlite3 *db);
cql_code test_bytebuf_growth(sqlite3 *db);
cql_code test_cql_finalize_on_error(sqlite3 *db);
cql_code test_blob_rowsets(sqlite3 *db);
cql_code test_sparse_blob_rowsets(sqlite3 *db);
cql_code test_c_one_row_result(sqlite3 *db);
cql_code test_ref_comparisons(sqlite3 *db);
cql_code test_all_column_fetchers(sqlite3 *db);
cql_code test_error_case_rowset(sqlite3 *db);
cql_code test_autodrop_rowset(sqlite3 *db);
cql_code test_one_row_result(sqlite3 *db);
cql_code test_cql_bytebuf_open(sqlite3 *db);
cql_code test_cql_bytebuf_alloc_within_bytebuf_exp_growth_cap(sqlite3 *db);
cql_code test_cql_bytebuf_alloc_over_bytebuf_exp_growth_cap(sqlite3 *db);
cql_code test_all_column_encoded_fetchers(sqlite3 *db);
cql_code test_all_column_encoded_cursor(sqlite3 *db);
cql_code test_all_column_encoded_out_union(sqlite3 *db);
cql_code test_all_column_encoded_multi_out_union(sqlite3 *db);
cql_code test_all_column_some_encoded_field(sqlite3 *db);
cql_code test_all_column_encoded_runtime_turn_on_off(sqlite3 *db);

static int32_t steps_until_fail = 0;
static int32_t trace_received = 0;

#undef sqlite3_step

#define _VA_ARG_COUNT__(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _count, ...) _count
#define _VA_ARG_COUNT_(...) _VA_ARG_COUNT__(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _VA_ARG_COUNT(...) _VA_ARG_COUNT_(0, ##__VA_ARGS__)
#define _VA_ARG_FIRST(_first, ...) _first

#define _E(cond_, rc_, ...) { \
  expectations++; \
  if (!(cond_)) { \
    fails++; \
    if (_VA_ARG_COUNT(__VA_ARGS__) != 0 && (_VA_ARG_FIRST(__VA_ARGS__)) != NULL) { \
      printf(__VA_ARGS__); \
    } \
    return rc_; \
  } \
}

#define E(cond_, ...) _E(cond_, SQLITE_ERROR, __VA_ARGS__)

#define SQL_E(rc_) { \
  cql_code __saved_rc_ = (rc_); \
  _E(SQLITE_OK == __saved_rc_, __saved_rc_, "failed return code %s:%d %s\n", __FILE__, __LINE__, #rc_); \
}

cql_code mockable_sqlite3_step(sqlite3_stmt *stmt) {
  if (steps_until_fail) {
    if (0 == --steps_until_fail) {
      return SQLITE_ERROR;
    }
  }
  return sqlite3_step(stmt);
}


cql_code run_client(sqlite3 *db) {

  E(longs__one == 1L, "long_enum case 1");
  E(longs__big == 0x100000000L, "long_enum case 2");
  E(floats__one == 1.0, "float_enum case 1");
  E(floats__two == 2.0, "float_enum case 2");

  E(fails_because_bogus_table(db) != SQLITE_OK, "procedure should have returned an error\n");
  E(trace_received == 1, "failure proc did not trigger a trace\n");
  E(!cql_outstanding_refs, "outstanding refs in fails_because_bogus_table: %d\n", cql_outstanding_refs);

  SQL_E(test_c_rowsets(db));
  E(!cql_outstanding_refs, "outstanding refs in test_c_rowsets: %d\n", cql_outstanding_refs);

  SQL_E(test_rowset_same(db));
  E(!cql_outstanding_refs, "outstanding refs in test_rowset_same: %d\n", cql_outstanding_refs);

  SQL_E(test_blob_rowsets(db));
  E(!cql_outstanding_refs, "outstanding refs in test_blob_rowsets: %d\n", cql_outstanding_refs);

  SQL_E(test_ref_comparisons(db));
  E(!cql_outstanding_refs, "outstanding refs in test_ref_comparisons: %d\n", cql_outstanding_refs);

  SQL_E(test_sparse_blob_rowsets(db));
  E(!cql_outstanding_refs, "outstanding refs in test_sparse_blob_rowsets: %d\n", cql_outstanding_refs);

  SQL_E(test_bytebuf_growth(db));
  E(!cql_outstanding_refs, "outstanding refs in test bytebuf growth: %d\n", cql_outstanding_refs);

  SQL_E(test_cql_finalize_on_error(db));
  E(!cql_outstanding_refs, "outstanding refs in test finalize on error: %d\n", cql_outstanding_refs);

  SQL_E(test_c_one_row_result(db));
  E(!cql_outstanding_refs, "outstanding refs in test_c_one_row_result: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_fetchers(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_fetchers: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_encoded_fetchers(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_encoded_fetchers: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_encoded_cursor(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_encoded_cursor: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_encoded_out_union(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_encoded_out_union: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_encoded_multi_out_union(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_encoded_multi_out_union: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_encoded_runtime_turn_on_off(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_encoded_runtime_turn_on_off: %d\n", cql_outstanding_refs);

  SQL_E(test_all_column_some_encoded_field(db));
  E(!cql_outstanding_refs, "outstanding refs in test_all_column_some_encoded_field: %d\n", cql_outstanding_refs);

  SQL_E(test_error_case_rowset(db));
  E(!cql_outstanding_refs, "outstanding refs in test_error_case_rowset: %d\n", cql_outstanding_refs);

  SQL_E(test_autodrop_rowset(db));
  E(!cql_outstanding_refs, "outstanding refs in test_autodrop_rowset (run 1): %d\n", cql_outstanding_refs);

  SQL_E(test_autodrop_rowset(db));
  E(!cql_outstanding_refs, "outstanding refs in test_autodrop_rowset (run 2): %d\n", cql_outstanding_refs);

  SQL_E(test_one_row_result(db));
  E(!cql_outstanding_refs, "outstanding refs in one_row_result: %d\n", cql_outstanding_refs);

  SQL_E(test_cql_bytebuf_open(db));
  E(!cql_outstanding_refs, "outstanding refs in test_cql_bytebuf_open: %d\n", cql_outstanding_refs);

  SQL_E(test_cql_bytebuf_alloc_within_bytebuf_exp_growth_cap(db));
  E(!cql_outstanding_refs,
    "outstanding refs in test_cql_bytebuf_alloc_within_bytebuf_exp_growth_cap: %d\n",
    cql_outstanding_refs);

  SQL_E(test_cql_bytebuf_alloc_over_bytebuf_exp_growth_cap(db));
  E(!cql_outstanding_refs,
    "outstanding refs in test_cql_bytebuf_alloc_over_bytebuf_exp_growth_cap: %d\n",
    cql_outstanding_refs);

  return SQLITE_OK;
}

void run_test_trace_callback(const char *proc, const char *file, int32_t line) {
  if (strcmp(proc, "fails_because_bogus_table"))  {
    printf("failure trace not recieved on correct proc\n");
    return;
  }

  if (!file || strlen(file) < 8) {
    printf("callback file not a reasonable value\n");
    return;
  }

  if (line < 1000) {
    printf("line number in callback not a reasonable value\n");
    return;
  }

  trace_received++;
}

cql_code test_c_rowsets(sqlite3 *db) {
  printf("Running C client test\n");
  tests++;

  // we haven't created the table yet
  SQL_E(drop_mixed(db));
  get_mixed_result_set_ref result_set;
  E(SQLITE_OK != get_mixed_fetch_results(db, &result_set, 100), "table didn't exist, yet there was data...\n");

  SQL_E(make_mixed(db));
  SQL_E(load_mixed_with_nulls(db));
  SQL_E(get_mixed_fetch_results(db, &result_set, 100));

  cql_int32 count = get_mixed_result_count(result_set);
  E(count == 4, "expected 4 rows from mixed\n");

  cql_bool b_is_null;
  cql_bool b;
  cql_bool code_is_null;
  cql_int64 code;
  cql_string_ref name;

  b_is_null = get_mixed_get_flag_is_null(result_set, 0);
  b = get_mixed_get_flag_value(result_set, 0);
  code_is_null = get_mixed_get_code_is_null(result_set, 0);
  code = get_mixed_get_code_value(result_set, 0);
  name = get_mixed_get_name(result_set, 0);

  E(b_is_null == 0, "first mixed row has unexpected b_is_null value\n");
  E(b == 1, "first mixed row has unexpected b value\n");
  E(code_is_null == 0, "first mixed row has unexpected code_is_null value\n");
  E(code == 12, "first mixed row has unexpected code value\n");
  E(strcmp("a name", name->ptr) == 0, "first mixed row has unexpected name value\n");

  b_is_null = get_mixed_get_flag_is_null(result_set, 1);
  b = get_mixed_get_flag_value(result_set, 1);
  code_is_null = get_mixed_get_code_is_null(result_set, 1);
  code = get_mixed_get_code_value(result_set, 1);
  name = get_mixed_get_name(result_set, 1);

  E(b_is_null == 0, "second mixed row has unexpected b_is_null value\n");
  E(b == 1, "second mixed row has unexpected b value\n");
  E(code_is_null == 0, "second mixed row has unexpected code_is_null value\n");
  E(code == 14, "second mixed row has unexpected code value\n");
  E(strcmp("another name", name->ptr) == 0, "second mixed row has unexpected name value\n");

  // don't get b_value and code_value, as that will assert, since they are NULL
  b_is_null = get_mixed_get_flag_is_null(result_set, 2);
  code_is_null = get_mixed_get_code_is_null(result_set, 2);
  name = get_mixed_get_name(result_set, 2);

  E(b_is_null == 1, "third mixed row has unexpected b_is_null value\n");
  E(code_is_null == 1, "third mixed row has unexpected code_is_null value\n");
  E(name == NULL, "third mixed row has unexpected name value\n");

  b_is_null = get_mixed_get_flag_is_null(result_set, 3);
  b = get_mixed_get_flag_value(result_set, 3);
  code_is_null = get_mixed_get_code_is_null(result_set, 3);
  code = get_mixed_get_code_value(result_set, 3);
  name = get_mixed_get_name(result_set, 3);

  E(b_is_null == 0, "fourth mixed row has unexpected b_is_null value\n");
  E(b == 0, "fourth mixed row has unexpected b value\n");
  E(code_is_null == 0, "fourth mixed row has unexpected code_is_null value\n");
  E(code == 16, "fourth mixed row has unexpected code value\n");
  E(strcmp("last name", name->ptr) == 0, "fourth mixed row has unexpected name value\n");

  cql_int32 copy_index = 1;
  cql_int32 copy_count = 2;
  get_mixed_result_set_ref result_set_copy;
  get_mixed_copy(result_set, &result_set_copy, copy_index, copy_count);

  for (cql_int32 i = 0; i < copy_count; ++i) {
    // Check that the row hashes are equal from the source to the copy
    E(get_mixed_row_hash(result_set, i + copy_index) == get_mixed_row_hash(result_set_copy, i),
      "copied row %d hash not equal to the expected source row %d\n", i, i + copy_index);

    // Check that the rows are equal from the source to the copy
    E(get_mixed_row_equal(result_set, i + copy_index, result_set_copy, i),
      "copied row %d not equal to the expected source row %d\n", i, i + copy_index);
  }

  for (cql_int32 i = 0; i < count - 1; ++i) {
    // Check that the wrong rows are not equal
    E(!get_mixed_row_equal(result_set, i, result_set, i + 1), "row %d should not be equal to row %d\n", i, i + 1);

    // Check that strings are not equal
    E(!cql_string_equal(get_mixed_get_name(result_set, i), get_mixed_get_name(result_set, i + 1)),
      "row %d name should not be equal to row %d name\n", i, i + 1);
  }

  cql_result_set_release(result_set);
  cql_result_set_release(result_set_copy);

  tests_passed++;
  return SQLITE_OK;
}

static cql_nullable_int64 make_nullable_code(cql_bool is_null, cql_int64 value) {
  cql_nullable_int64 code;
  cql_set_nullable(code, is_null, value);
  return code;
}

static cql_nullable_int64 get_nullable_code(get_mixed_result_set_ref result_set, int32_t row) {
  return make_nullable_code(get_mixed_get_code_is_null(result_set, row), get_mixed_get_code_value(result_set, row));
}

cql_code test_rowset_same(sqlite3 *db) {
  printf("Running cql_row_same client test\n");
  tests++;

  get_mixed_result_set_ref result_set;
  get_mixed_result_set_ref result_set_updated;

  cql_string_ref updated_name = string_create();
  cql_blob_ref updated_bl = blob_from_string(updated_name);

  SQL_E(drop_mixed(db));
  SQL_E(make_mixed(db));
  SQL_E(load_mixed_with_nulls(db));
  SQL_E(get_mixed_fetch_results(db, &result_set, 100));

  // Update row 0 with just a new name, so the identity columns should still match the previous result set
  SQL_E(update_mixed(db,
                     get_mixed_get_id(result_set, 0),
                     updated_name,
                     get_nullable_code(result_set, 0),
                     get_mixed_get_bl(result_set, 0)));


  // Update row 1 with just a new code, so only 1 of the identity columns should not match the previous result set
  SQL_E(update_mixed(db,
                     get_mixed_get_id(result_set, 1),
                     get_mixed_get_name(result_set, 1),
                     make_nullable_code(0, 1234),
                     get_mixed_get_bl(result_set, 1)));

  // Update row 2 with just a new bl, so a ref type identity column should not match the previous result set
  // This also is testing that the last column in the identity columns is properly tested.
  SQL_E(update_mixed(db,
                     get_mixed_get_id(result_set, 2),
                     get_mixed_get_name(result_set, 2),
                     get_nullable_code(result_set, 2),
                     updated_bl));

  // Get the updated result set
  SQL_E(get_mixed_fetch_results(db, &result_set_updated, 100));

  E(get_mixed_row_same(result_set, 0, result_set_updated, 0),
    "updated row 0 should be the same as original row 0 (identity column check)\n");
  E(!get_mixed_row_same(result_set, 1, result_set_updated, 1),
    "updated row 1 should NOT be the same as original row 1 (identity column check)\n");
  E(!get_mixed_row_same(result_set, 2, result_set_updated, 2),
    "updated row 2 should NOT be the same as original row 2 (identity column check)\n");
  E(!get_mixed_row_same(result_set, 0, result_set_updated, 1),
    "updated row 1 should NOT be the same as original row 0 (identity column check)\n");

  cql_result_set_release(result_set);
  cql_result_set_release(result_set_updated);
  cql_string_release(updated_name);
  cql_blob_release(updated_bl);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_ref_comparisons(sqlite3 *db) {
  printf("Running ref comparison test\n");
  tests++;

  // we haven't created the table yet
  get_mixed_result_set_ref result_set;
  SQL_E(load_mixed_dupes(db));

  SQL_E(get_mixed_fetch_results(db, &result_set, 100));
  E(get_mixed_result_count(result_set) == 6, "expected 6 rows from mixed\n");

  // Check that the row hashes are equal from the source to the copy
  E(get_mixed_row_hash(result_set, 0) == get_mixed_row_hash(result_set, 1), "row %d hash not equal to row %d\n", 0, 1);
  E(get_mixed_row_hash(result_set, 2) == get_mixed_row_hash(result_set, 3), "row %d hash not equal to row %d\n", 2, 3);

  E(get_mixed_row_equal(result_set, 0, result_set, 1), "row %d should be equal to row %d\n", 0, 1);
  E(get_mixed_row_equal(result_set, 2, result_set, 3), "row %d should be equal to row %d\n", 2, 3);
  E(!get_mixed_row_equal(result_set, 0, result_set, 2), "row %d should be not equal to row %d\n", 0, 2);
  E(!get_mixed_row_equal(result_set, 1, result_set, 3), "row %d should not be equal to row %d\n", 1, 3);
  E(!get_mixed_row_equal(result_set, 0, result_set, 4), "row %d should not be equal to row %d\n", 0, 4);
  E(!get_mixed_row_equal(result_set, 1, result_set, 5), "row %d should not be equal to row %d\n", 0, 4);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_bytebuf_growth(sqlite3 *db) {
  tests++;
  printf("Running C client test with huge number of rows\n");

  SQL_E(bulk_load_mixed(db, 10000));

  get_mixed_result_set_ref result_set;
  SQL_E(get_mixed_fetch_results(db, &result_set, 100000));
  E(get_mixed_result_count(result_set) == 10000, "expected 10000 rows from mixed\n");

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_cql_finalize_on_error(sqlite3 *db) {
  printf("Running cql finalize on error test\n");
  tests++;

  expectations++;
  SQL_E(load_mixed(db));

  sqlite3_stmt *stmt = NULL;
  SQL_E(sqlite3_prepare_v2(db, "select * from sqlite_master", -1, &stmt, NULL));

  cql_finalize_on_error(SQLITE_ERROR,&stmt);
  E(stmt == NULL, "expected statement to be finalized\n");

  tests_passed++;
  return SQLITE_OK;
}

cql_string_ref _Nullable string_create()
{
  return cql_string_ref_new("Hello, world.");
}

cql_int32 run_test_math(cql_int32 int1, cql_nullable_int32 *_Nonnull int2)
{
   int2->is_null = 0;
   int2->value = int1 * 5;
   return int1 * 7;
}

// dumbest set ever
#define MAX_STRINGS 16
typedef struct set_payload
{
  int count;
  cql_string_ref strings[MAX_STRINGS];
} set_payload;

static void set_finalize(cql_type_ref _Nonnull ref)
{
  cql_object_ref obj = (cql_object_ref)ref;
  set_payload *payload = (set_payload *)obj->ptr;
  for (int i = 0; i < payload->count; i++) {
    cql_set_string_ref(&payload->strings[i], NULL);
  }
  free(payload);
  obj->ptr = NULL;
}

cql_object_ref _Nonnull set_create()
{
  cql_object_ref obj = (cql_object_ref)calloc(sizeof(cql_object), 1);
  obj->base.ref_count = 1;
  obj->base.finalize = set_finalize;
  obj->ptr = calloc(sizeof(set_payload), 1);
  cql_outstanding_refs++;
  return obj;
}

cql_bool set_contains(cql_object_ref _Nonnull _set, cql_string_ref _Nonnull _key)
{
  set_payload *payload = (set_payload *)_set->ptr;

  for (int i = 0; i < payload->count; i++) {
    if (!strcmp(_key->ptr, payload->strings[i]->ptr)) {
      return cql_true;
    }
  }
  return cql_false;
}

cql_bool set_add(cql_object_ref _Nonnull _set, cql_string_ref _Nonnull _key)
{
  if (set_contains(_set, _key)) {
    return cql_false;
  }

  set_payload *payload = (set_payload *)_set->ptr;
  if (payload->count >= MAX_STRINGS) {
    return cql_false;
  }

  cql_set_string_ref(&payload->strings[payload->count], _key);
  payload->count++;

  return cql_true;
}

int get_outstanding_refs()
{
  return cql_outstanding_refs;
}

cql_blob_ref _Nonnull blob_from_string(cql_string_ref str)
{
  if (str) {
    return cql_blob_ref_new(str->ptr, strlen(str->ptr));
  }
  else {
    return cql_blob_ref_new("", 1);
  }
}

cql_string_ref _Nonnull string_from_blob(cql_blob_ref b)
{
  if (b) {
    char *buf = alloca(b->size+1);
    memcpy(buf, b->ptr, b->size);
    buf[b->size] = 0;
    return cql_string_ref_new(buf);
  }
  else {
    return cql_string_ref_new("");
  }
}

cql_code test_blob_rowsets(sqlite3 *db) {
  printf("Running blob rowset test\n");
  tests++;

  SQL_E(load_blobs(db));

  get_blob_table_result_set_ref result_set;
  SQL_E(get_blob_table_fetch_results(db, &result_set));

  E(get_blob_table_result_count(result_set) == 20, "expected 20 rows from blob table\n");

  cql_int32 id;
  cql_blob_ref b1;
  cql_blob_ref b2;

  for (cql_int32 i = 0; i < 20; i++) {
    id = get_blob_table_get_id(result_set, i);
    b1 = get_blob_table_get_b1(result_set, i);
    b2 = get_blob_table_get_b2(result_set, i);

    E(i == id, "id %d did not match %d\n", id, i);

    char buf1[100];
    char buf2[100];

    sprintf(buf1, "nullable blob %d", i);
    sprintf(buf2, "not nullable blob %d", i);

    cql_string_ref b1_ref = string_from_blob(b1);
    cql_string_ref b2_ref = string_from_blob(b2);
    E(strcmp(buf1, b1_ref->ptr) == 0, "nullable blob %d did not match %s\n", i, buf1);
    E(strcmp(buf2, b2_ref->ptr) == 0, "nullable blob %d did not match %s\n", i, buf2);
    cql_string_release(b1_ref);
    cql_string_release(b2_ref);
  }

  cql_result_set_release(result_set);

  cql_string_ref str_ref = cql_string_ref_new("123");
  cql_blob_ref blob_ref = blob_from_string(str_ref);
  cql_string_ref str_ref_1 = string_from_blob(blob_ref);
  cql_blob_ref blob_ref_1 = blob_from_string(str_ref_1);
  E(cql_string_equal(str_ref, str_ref_1), "string \"%s\" should equal to \"%s\"", str_ref->ptr, str_ref_1->ptr);
  E(cql_blob_equal(blob_ref, blob_ref_1), "blob \"%d\" should be equal to \"%d\"", blob_ref->size, blob_ref_1->size);

  cql_string_release(str_ref);
  cql_string_release(str_ref_1);
  cql_blob_release(blob_ref);
  cql_blob_release(blob_ref_1);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_sparse_blob_rowsets(sqlite3 *db) {
  printf("Running sparse blob rowset test\n");
  tests++;

  SQL_E(load_sparse_blobs(db));

  get_blob_table_result_set_ref result_set;
  SQL_E(get_blob_table_fetch_results(db, &result_set));

  E(get_blob_table_result_count(result_set) == 20, "expected 20 rows from blob table\n");

  cql_int32 id;
  cql_blob_ref b1;
  cql_blob_ref b2;

  cql_hash_code prev = -1;

  for (cql_int32 i = 0; i < 20; i++) {
    id = get_blob_table_get_id(result_set, i);
    b1 = get_blob_table_get_b1(result_set, i);
    b2 = get_blob_table_get_b2(result_set, i);

    cql_hash_code h = get_blob_table_row_hash(result_set, i);

    E(h != prev, "hash codes really shouldn't collide so easily (row %d)\n", i);
    prev = h;

    E(i == id, "id %d did not match %d\n", id, i);

    char buf1[100];
    char buf2[100];

    sprintf(buf1, "nullable blob %d", i);
    sprintf(buf2, "not nullable blob %d", i);

    cql_string_ref b1_ref = string_from_blob(b1);
    if (i % 2 == 0) {
      E(strcmp(buf1, b1_ref->ptr) == 0, "nullable blob %d did not match %s\n", i, buf1);
    } else {
      E(!b1, "nullable blob %d should have been null\n", i);
    }

    cql_string_ref b2_ref = string_from_blob(b2);
    E(strcmp(buf2, b2_ref->ptr) == 0, "nullable blob %d did not match %s\n", i, buf2);

    cql_string_release(b1_ref);
    cql_string_release(b2_ref);
  }

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_c_one_row_result(sqlite3 *db) {
  printf("Running C one row result set test\n");
  tests++;

  SQL_E(drop_mixed(db));

  // we haven't created the table yet
  get_one_from_mixed_result_set_ref result_set;
  E(SQLITE_OK != get_one_from_mixed_fetch_results(db, &result_set, 1), "table didn't exist, yet there was data...\n");
  SQL_E(make_mixed(db));
  SQL_E(load_mixed_with_nulls(db));

  SQL_E(get_one_from_mixed_fetch_results(db, &result_set, 1));
  E(get_one_from_mixed_result_count(result_set) == 1, "expected 1 rows from mixed\n");

  cql_bool b_is_null;
  cql_bool b;
  cql_bool code_is_null;
  cql_int64 code;
  cql_string_ref name;

  b_is_null = get_one_from_mixed_get_flag_is_null(result_set);
  b = get_one_from_mixed_get_flag_value(result_set);
  code_is_null = get_one_from_mixed_get_code_is_null(result_set);
  code = get_one_from_mixed_get_code_value(result_set);
  name = get_one_from_mixed_get_name(result_set);

  E(b_is_null == 0, "mixed row has unexpected b_is_null value\n");
  E(b == 1, "mixed row has unexpected b value\n");
  E(code_is_null == 0, "mixed row has unexpected code_is_null value\n");
  E(code == 12, "mixed row has unexpected code value\n");
  E(strcmp("a name", name->ptr) == 0, "mixed row has unexpected name value\n");

  // Compare to a result from a row with different values
  get_one_from_mixed_result_set_ref result_set2;
  SQL_E(get_one_from_mixed_fetch_results(db, &result_set2, 2));
  E(get_one_from_mixed_result_count(result_set2) == 1, "expected 1 rows from mixed\n");

  cql_bool b_is_null2;
  cql_bool b2;
  cql_bool code_is_null2;
  cql_int64 code2;
  cql_string_ref name2;

  b_is_null2 = get_one_from_mixed_get_flag_is_null(result_set2);
  b2 = get_one_from_mixed_get_flag_value(result_set2);
  code_is_null2 = get_one_from_mixed_get_code_is_null(result_set2);
  code2 = get_one_from_mixed_get_code_value(result_set2);
  name2 = get_one_from_mixed_get_name(result_set2);

  E(b_is_null2 == 0, "mixed row 2 has unexpected b_is_null value\n");
  E(b2 == 1, "mixed row 2 has unexpected b value\n");
  E(code_is_null2 == 0, "mixed row 2 has unexpected code_is_null value\n");
  E(code2 == 14, "mixed row 2 has unexpected code value\n");
  E(strcmp("another name", name2->ptr) == 0, "mixed row 2 has unexpected name value\n");

  E(!get_one_from_mixed_equal(result_set, result_set2), "mismatched result sets are equal\n");
  E(get_one_from_mixed_hash(result_set) != get_one_from_mixed_hash(result_set2),
    "mismatched result sets have the same hashes\n");

  cql_result_set_release(result_set2);

  // Exercise single-row copy and compare copied results
  get_one_from_mixed_copy(result_set, &result_set2);

  E(get_one_from_mixed_equal(result_set, result_set2), "result set copies are not equal\n");
  E(get_one_from_mixed_hash(result_set) == get_one_from_mixed_hash(result_set2),
    "result set copies do not have the same hashes\n");

  cql_result_set_release(result_set2);

  // Compare results fetched from the same row
  SQL_E(get_one_from_mixed_fetch_results(db, &result_set2, 1));

  E(get_one_from_mixed_equal(result_set, result_set2), "result sets for same row are not equal\n");
  E(get_one_from_mixed_hash(result_set) == get_one_from_mixed_hash(result_set2),
    "result sets for same row do not have the same hashes\n");

  cql_result_set_release(result_set);
  cql_result_set_release(result_set2);

  SQL_E(get_one_from_mixed_fetch_results(db, &result_set, 999));
  E(get_one_from_mixed_result_count(result_set) == 0, "expected 0 rows from mixed\n");

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_encoded_fetchers(sqlite3 *db) {
  printf("Running column encoded fetchers test\n");
  tests++;

  load_encoded_table_result_set_ref result_set;
  SQL_E(load_encoded_table_fetch_results(db, &result_set));
  E(load_encoded_table_result_count(result_set) == 1, "expected 1 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  load_encoded_table_result_set_ref rs = (load_encoded_table_result_set_ref)result_set;

  cql_bool b0 = load_encoded_table_get_b0_value(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  cql_bool b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);

  cql_int32 i0 = load_encoded_table_get_i0_value(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 1), "expected i0 is encoded\n");
  cql_int32 i0_exp = cql_decode_int32(db, i0);
  E(i0_exp == 0, "expected i0 is 0, value %d\n", i0_exp);

  cql_int64 l0 = load_encoded_table_get_l0_value(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 2), "expected l0 is encoded\n");
  cql_int64 l0_exp = cql_decode_int64(db, l0);
  E(l0_exp == 0, "expected l0 is 0, value %lld\n", l0_exp);

  cql_double d0 = load_encoded_table_get_d0_value(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 3), "expected d0 is encoded\n");
  cql_double d0_exp = cql_decode_double(db, d0);
  E(d0_exp == 0.0, "expected d0 is 0.0, value %lf\n", d0_exp);


  cql_string_ref s0 = load_encoded_table_get_s0(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 4), "expected s0 is encoded\n");
  cql_string_ref s0_decode = cql_decode_string_ref_new(db, s0);
  cql_string_ref s0_exp = cql_string_ref_new("0");
  E(cql_string_equal(s0_decode, s0_exp), "expected s0 is 0, value %s\n", s0_decode->ptr);
  cql_string_release(s0_decode);
  cql_string_release(s0_exp);

  cql_blob_ref bl0 = load_encoded_table_get_bl0(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 5), "expected bl0 is encoded\n");
  cql_blob_ref bl0_decode = cql_decode_blob_ref_new(db, bl0);
  cql_blob_ref bl0_exp = cql_blob_ref_new("0", 1);
  E(cql_blob_equal(bl0_decode, bl0_exp), "expected bl0 is \"0\", value %s\n", (const char*)bl0_decode->ptr);
  cql_blob_release(bl0_decode);
  cql_blob_release(bl0_exp);

  cql_bool b1 = load_encoded_table_get_b1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 6), "expected b1 is encoded\n");
  cql_bool b1_exp = cql_decode_bool(db, b1);
  E(b1_exp == 1, "expected b1 is 1, value %d\n", b1_exp);

  cql_int32 i1 = load_encoded_table_get_i1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 7), "expected i1 is encoded\n");
  cql_int32 i1_exp = cql_decode_int32(db, i1);
  E(i1_exp == 1, "expected i1 is 1, value %d\n", i1_exp);

  cql_int64 l1 = load_encoded_table_get_l1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 8), "expected l1 is encoded\n");
  cql_int64 l1_exp = cql_decode_int64(db, l1);
  E(l1_exp == 1, "expected l1 is 1, value %lld\n", l1_exp);

  cql_double d1 = load_encoded_table_get_d1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 9), "expected d1 is encoded\n");
  cql_double d1_exp = cql_decode_double(db, d1);
  E(d1_exp == 1.1, "expected d1 is 1.1, value %lf\n", d1_exp);

  cql_string_ref s1 = load_encoded_table_get_s1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 10), "expected s1 is encoded\n");
  cql_string_ref s1_decode = cql_decode_string_ref_new(db, s1);
  cql_string_ref s1_exp = cql_string_ref_new("1");
  E(cql_string_equal(s1_decode, s1_exp), "expected s1 is 1, value %s\n", s1_decode->ptr);
  cql_string_release(s1_decode);
  cql_string_release(s1_exp);

  cql_blob_ref bl1 = load_encoded_table_get_bl1(rs, 0);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 11), "expected bl1 is encoded\n");
  cql_blob_ref bl1_decode = cql_decode_blob_ref_new(db, bl1);
  cql_blob_ref bl1_exp = cql_blob_ref_new("1", 1);
  E(cql_blob_equal(bl1_decode, bl1_exp), "expected bl1 is \"1\", value %s\n", (const char*)bl1_decode->ptr);
  cql_blob_release(bl1_decode);
  cql_blob_release(bl1_exp);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_encoded_cursor(sqlite3 *db) {
  printf("Running column encoded cursor fetchers test\n");
  tests++;

  load_encoded_cursor_result_set_ref result_set;
  SQL_E(load_encoded_cursor_fetch_results(db, &result_set));
  E(load_encoded_cursor_result_count(result_set) == 1, "expected 1 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  load_encoded_cursor_result_set_ref rs = (load_encoded_cursor_result_set_ref)result_set;

  cql_bool b0 = load_encoded_cursor_get_b0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  cql_bool b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);

  cql_int32 i0 = load_encoded_cursor_get_i0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 1), "expected i0 is encoded\n");
  cql_int32 i0_exp = cql_decode_int32(db, i0);
  E(i0_exp == 0, "expected i0 is 0, value %d\n", i0_exp);

  cql_int64 l0 = load_encoded_cursor_get_l0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 2), "expected l0 is encoded\n");
  cql_int64 l0_exp = cql_decode_int64(db, l0);
  E(l0_exp == 0, "expected l0 is 0, value %lld\n", l0_exp);

  cql_double d0 = load_encoded_cursor_get_d0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 3), "expected d0 is encoded\n");
  cql_double d0_exp = cql_decode_double(db, d0);
  E(d0_exp == 0.0, "expected d0 is 0.0, value %lf\n", d0_exp);


  cql_string_ref s0 = load_encoded_cursor_get_s0(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 4), "expected s0 is encoded\n");
  cql_string_ref s0_decode = cql_decode_string_ref_new(db, s0);
  cql_string_ref s0_exp = cql_string_ref_new("0");
  E(cql_string_equal(s0_decode, s0_exp), "expected s0 is 0, value %s\n", s0_decode->ptr);
  cql_string_release(s0_decode);
  cql_string_release(s0_exp);

  cql_blob_ref bl0 = load_encoded_cursor_get_bl0(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 5), "expected bl0 is encoded\n");
  cql_blob_ref bl0_decode = cql_decode_blob_ref_new(db, bl0);
  cql_blob_ref bl0_exp = cql_blob_ref_new("0", 1);
  E(cql_blob_equal(bl0_decode, bl0_exp), "expected bl0 is \"0\", value %s\n", (const char*)bl0_decode->ptr);
  cql_blob_release(bl0_decode);
  cql_blob_release(bl0_exp);

  cql_bool b1 = load_encoded_cursor_get_b1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 6), "expected b1 is encoded\n");
  cql_bool b1_exp = cql_decode_bool(db, b1);
  E(b1_exp == 1, "expected b1 is 1, value %d\n", b1_exp);

  cql_int32 i1 = load_encoded_cursor_get_i1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 7), "expected i1 is encoded\n");
  cql_int32 i1_exp = cql_decode_int32(db, i1);
  E(i1_exp == 1, "expected i1 is 1, value %d\n", i1_exp);

  cql_int64 l1 = load_encoded_cursor_get_l1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 8), "expected l1 is encoded\n");
  cql_int64 l1_exp = cql_decode_int64(db, l1);
  E(l1_exp == 1, "expected l1 is 1, value %lld\n", l1_exp);

  cql_double d1 = load_encoded_cursor_get_d1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 9), "expected d1 is encoded\n");
  cql_double d1_exp = cql_decode_double(db, d1);
  E(d1_exp == 1.1, "expected d1 is 1.1, value %lf\n", d1_exp);

  cql_string_ref s1 = load_encoded_cursor_get_s1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 10), "expected s1 is encoded\n");
  cql_string_ref s1_decode = cql_decode_string_ref_new(db, s1);
  cql_string_ref s1_exp = cql_string_ref_new("1");
  E(cql_string_equal(s1_decode, s1_exp), "expected s1 is 1, value %s\n", s1_decode->ptr);
  cql_string_release(s1_decode);
  cql_string_release(s1_exp);

  cql_blob_ref bl1 = load_encoded_cursor_get_bl1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 11), "expected bl1 is encoded\n");
  cql_blob_ref bl1_decode = cql_decode_blob_ref_new(db, bl1);
  cql_blob_ref bl1_exp = cql_blob_ref_new("1", 1);
  E(cql_blob_equal(bl1_decode, bl1_exp), "expected bl1 is \"1\", value %s\n", (const char*)bl1_decode->ptr);
  cql_blob_release(bl1_decode);
  cql_blob_release(bl1_exp);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_encoded_out_union(sqlite3 *db) {
  printf("Running column encoded out union fetchers test\n");
  tests++;

  load_decoded_out_union_result_set_ref result_set;
  SQL_E(load_decoded_out_union_fetch_results(db, &result_set));
  E(load_decoded_out_union_result_count(result_set) == 1, "expected 1 row from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  load_decoded_out_union_result_set_ref rs = (load_decoded_out_union_result_set_ref)result_set;

  cql_bool b0 = load_decoded_out_union_get_b0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  cql_bool b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);

  cql_int32 i0 = load_decoded_out_union_get_i0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 1), "expected i0 is encoded\n");
  cql_int32 i0_exp = cql_decode_int32(db, i0);
  E(i0_exp == 0, "expected i0 is 0, value %d\n", i0_exp);

  cql_int64 l0 = load_decoded_out_union_get_l0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 2), "expected l0 is encoded\n");
  cql_int64 l0_exp = cql_decode_int64(db, l0);
  E(l0_exp == 0, "expected l0 is 0, value %lld\n", l0_exp);

  cql_double d0 = load_decoded_out_union_get_d0_value(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 3), "expected d0 is encoded\n");
  cql_double d0_exp = cql_decode_double(db, d0);
  E(d0_exp == 0.0, "expected d0 is 0.0, value %lf\n", d0_exp);

  cql_string_ref s0 = load_decoded_out_union_get_s0(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 4), "expected s0 is encoded\n");
  cql_string_ref s0_decode = cql_decode_string_ref_new(db, s0);
  cql_string_ref s0_exp = cql_string_ref_new("0");
  E(cql_string_equal(s0_decode, s0_exp), "expected s0 is 0, value %s\n", s0_decode->ptr);
  cql_string_release(s0_decode);
  cql_string_release(s0_exp);

  cql_blob_ref bl0 = load_decoded_out_union_get_bl0(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 5), "expected bl0 is encoded\n");
  cql_blob_ref bl0_decode = cql_decode_blob_ref_new(db, bl0);
  cql_blob_ref bl0_exp = cql_blob_ref_new("0", 1);
  E(cql_blob_equal(bl0_decode, bl0_exp), "expected bl0 is \"0\", value %s\n", (const char *)bl0_decode->ptr);
  cql_blob_release(bl0_decode);
  cql_blob_release(bl0_exp);

  cql_bool b1 = load_decoded_out_union_get_b1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 6), "expected b1 is encoded\n");
  cql_bool b1_exp = cql_decode_bool(db, b1);
  E(b1_exp == 1, "expected b1 is 1, value %d\n", b1_exp);

  cql_int32 i1 = load_decoded_out_union_get_i1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 7), "expected i1 is encoded\n");
  cql_int32 i1_exp = cql_decode_int32(db, i1);
  E(i1_exp == 1, "expected i1 is 1, value %d\n", i1_exp);

  cql_int64 l1 = load_decoded_out_union_get_l1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 8), "expected l1 is encoded\n");
  cql_int64 l1_exp = cql_decode_int64(db, l1);
  E(l1_exp == 1, "expected l1 is 1, value %lld\n", l1_exp);

  cql_double d1 = load_decoded_out_union_get_d1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 9), "expected d1 is encoded\n");
  cql_double d1_exp = cql_decode_double(db, d1);
  E(d1_exp == 1.1, "expected d1 is 1.1, value %lf\n", d1_exp);

  cql_string_ref s1 = load_decoded_out_union_get_s1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 10), "expected s1 is encoded\n");
  cql_string_ref s1_decode = cql_decode_string_ref_new(db, s1);
  cql_string_ref s1_exp = cql_string_ref_new("1");
  E(cql_string_equal(s1_decode, s1_exp), "expected s1 is 1, value %s\n", s1_decode->ptr);
  cql_string_release(s1_decode);
  cql_string_release(s1_exp);

  cql_blob_ref bl1 = load_decoded_out_union_get_bl1(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 11), "expected bl1 is encoded\n");
  cql_blob_ref bl1_decode = cql_decode_blob_ref_new(db, bl1);
  cql_blob_ref bl1_exp = cql_blob_ref_new("1", 1);
  E(cql_blob_equal(bl1_decode, bl1_exp), "expected bl1 is \"1\", value %s\n", (const char *)bl1_decode->ptr);
  cql_blob_release(bl1_decode);
  cql_blob_release(bl1_exp);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_encoded_runtime_turn_on_off(sqlite3 *db) {
  printf("Running column encoded turn off fetchers test\n");
  tests++;

  load_decoded_multi_out_union_result_set_ref result_set;
  SQL_E(load_decoded_multi_out_union_fetch_results(db, &result_set));
  E(load_decoded_multi_out_union_result_count(result_set) == 2, "expected 2 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  load_decoded_multi_out_union_result_set_ref rs = (load_decoded_multi_out_union_result_set_ref)result_set;

  cql_bool b0 = load_decoded_multi_out_union_get_b0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  cql_bool b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);
  cql_result_set_release(result_set);


  load_decoded_multi_out_union_set_encoding(0, false);
  SQL_E(load_decoded_multi_out_union_fetch_results(db, &result_set));
  rs = (load_decoded_multi_out_union_result_set_ref)result_set;

  b0 = load_decoded_multi_out_union_get_b0_value(rs, 1);
  E(!cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  E(b0 == 0, "expected b0 is 0, value %d\n", b0);
  cql_result_set_release(result_set);


  load_decoded_multi_out_union_set_encoding(0, true);
  SQL_E(load_decoded_multi_out_union_fetch_results(db, &result_set));
  rs = (load_decoded_multi_out_union_result_set_ref)result_set;

  b0 = load_decoded_multi_out_union_get_b0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);
  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_encoded_multi_out_union(sqlite3 *db) {
  printf("Running column encoded multi out union fetchers test\n");
  tests++;

  load_decoded_multi_out_union_result_set_ref result_set;
  SQL_E(load_decoded_multi_out_union_fetch_results(db, &result_set));
  E(load_decoded_multi_out_union_result_count(result_set) == 2, "expected 2 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  load_decoded_multi_out_union_result_set_ref rs = (load_decoded_multi_out_union_result_set_ref)result_set;

  cql_bool b0 = load_decoded_multi_out_union_get_b0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected b0 is encoded\n");
  E(load_decoded_multi_out_union_get_b0_is_encoded(rs), "expected b0 is encoded\n");
  cql_bool b0_exp = cql_decode_bool(db, b0);
  E(b0_exp == 0, "expected b0 is 0, value %d\n", b0_exp);

  cql_int32 i0 = load_decoded_multi_out_union_get_i0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 1), "expected i0 is encoded\n");
  cql_int32 i0_exp = cql_decode_int32(db, i0);
  E(i0_exp == 0, "expected i0 is 0, value %d\n", i0_exp);

  cql_int64 l0 = load_decoded_multi_out_union_get_l0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 2), "expected l0 is encoded\n");
  cql_int64 l0_exp = cql_decode_int64(db, l0);
  E(l0_exp == 0, "expected l0 is 0, value %lld\n", l0_exp);

  cql_double d0 = load_decoded_multi_out_union_get_d0_value(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 3), "expected dd0 is encoded\n");
  cql_double d0_exp = cql_decode_double(db, d0);
  E(d0_exp == 0.0, "expected d0 is 0.0, value %lf\n", d0_exp);

  cql_string_ref s0 = load_decoded_multi_out_union_get_s0(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 4), "expected s0 is encoded\n");
  cql_string_ref s0_decode = cql_decode_string_ref_new(db, s0);
  cql_string_ref s0_exp = cql_string_ref_new("0");
  E(cql_string_equal(s0_decode, s0_exp), "expected s0 is 0, value %s\n", s0_decode->ptr);
  cql_string_release(s0_decode);
  cql_string_release(s0_exp);

  cql_blob_ref bl0 = load_decoded_multi_out_union_get_bl0(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 5), "expected bl0 is encoded\n");
  cql_blob_ref bl0_decode = cql_decode_blob_ref_new(db, bl0);
  cql_blob_ref bl0_exp = cql_blob_ref_new("0", 1);
  E(cql_blob_equal(bl0_decode, bl0_exp), "expected bl0 is \"0\", value \"%s\"\n", (const char *)bl0_decode->ptr);
  cql_blob_release(bl0_decode);
  cql_blob_release(bl0_exp);

  cql_bool b1 = load_decoded_multi_out_union_get_b1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 6), "expected b1 is encoded\n");
  cql_bool b1_exp = cql_decode_bool(db, b1);
  E(b1_exp == 1, "expected b1 is 1, value %d\n", b1_exp);

  cql_int32 i1 = load_decoded_multi_out_union_get_i1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 7), "expected i1 is encoded\n");
  cql_int32 i1_exp = cql_decode_int32(db, i1);
  E(i1_exp == 1, "expected i1 is 1, value %d\n", i1_exp);

  cql_int64 l1 = load_decoded_multi_out_union_get_l1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 8), "expected l1 is encoded\n");
  cql_int64 l1_exp = cql_decode_int64(db, l1);
  E(l1_exp == 1, "expected l1 is 1, value %lld\n", l1_exp);

  cql_double d1 = load_decoded_multi_out_union_get_d1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 9), "expected d1 is encoded\n");
  cql_double d1_exp = cql_decode_double(db, d1);
  E(d1_exp == 1.1, "expected d1 is 1.1, value %lf\n", d1_exp);

  cql_string_ref s1 = load_decoded_multi_out_union_get_s1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 10), "expected s1 is encoded\n");
  cql_string_ref s1_decode = cql_decode_string_ref_new(db, s1);
  cql_string_ref s1_exp = cql_string_ref_new("1");
  E(cql_string_equal(s1_decode, s1_exp), "expected s1 is 1, value %s\n", s1_decode->ptr);
  cql_string_release(s1_decode);
  cql_string_release(s1_exp);

  cql_blob_ref bl1 = load_decoded_multi_out_union_get_bl1(rs, 1);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 11), "expected bl1 is encoded\n");
  cql_blob_ref bl1_decode = cql_decode_blob_ref_new(db, bl1);
  cql_blob_ref bl1_exp = cql_blob_ref_new("1", 1);
  E(cql_blob_equal(bl1_decode, bl1_exp), "expected bl1 is \"1\", value \"%s\"\n", (const char *)bl1_decode->ptr);
  cql_blob_release(bl1_decode);
  cql_blob_release(bl1_exp);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_some_encoded_field(sqlite3 *db) {
  printf("Running column some encoded field fetchers test\n");
  tests++;

  load_some_encoded_field_result_set_ref result_set;
  SQL_E(load_some_encoded_field_fetch_results(db, &result_set));
  E(load_some_encoded_field_result_count(result_set) == 1, "expected 1 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 2, "expected 2 columns from result table\n");
  load_some_encoded_field_result_set_ref rs = (load_some_encoded_field_result_set_ref)result_set;

  cql_int32 x = load_some_encoded_field_get_x_value(rs);
  E(!cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 0), "expected x is not encoded\n");
  E(x == 66, "expected x is 66, value %d\n", x);

  cql_string_ref y = load_some_encoded_field_get_y(rs);
  E(cql_result_set_get_is_encoded_col((cql_result_set_ref)rs, 1), "expected y is encoded\n");
  cql_string_ref y_decode = cql_decode_string_ref_new(db, y);
  cql_string_ref y_exp = cql_string_ref_new("bogus");
  E(cql_string_equal(y_decode, y_exp), "expected y is \"bogus\", value %s\n", y_decode->ptr);
  cql_string_release(y_decode);
  cql_string_release(y_exp);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_all_column_fetchers(sqlite3 *db) {
  printf("Running column fetchers test\n");
  tests++;

  load_all_types_table_result_set_ref result_set;
  SQL_E(load_all_types_table_fetch_results(db, &result_set));
  E(load_all_types_table_result_count(result_set) == 2, "expected 2 rows from result table\n");
  E(cql_result_set_get_meta(result_set)->columnCount == 12, "expected 12 columns from result table\n");
  cql_result_set_ref rs = (cql_result_set_ref)result_set;

  for (int32_t row = 0; row <= 1; row++) {
    for (int32_t col = 0; col < 12; col++) {
      cql_bool is_null = cql_result_set_get_is_null_col(rs, row, col);
      cql_bool is_null_expected = (row == 0) && col < 6;
      E(is_null == is_null_expected, "expected is_null did not match seed data, row %d, col %d\n", row, col);
    }
  }
  for (int32_t row = 0; row <= 1; row++) {
    for (int32_t col = 0; col < 12; col++) {
      cql_bool is_null_expected = (row == 0) && col < 6;
      if (is_null_expected) {
        continue;
      }
      switch (col % 6) {
        case 0:  {
          // bool
          E(cql_result_set_get_bool_col(rs, row, col) == row,
            "expected bool did not match seed data, row %d, col %d\n", row, col);
          break;
        }
        case 1: {
          // int32
          E(cql_result_set_get_int32_col(rs, row, col) == row,
            "expected int32 did not match seed data, row %d, col %d\n", row, col);
          break;
        }
        case 2: {
          // int64
          E(cql_result_set_get_int64_col(rs, row, col) == row,
            "expected int64 did not match seed data, row %d, col %d\n", row, col);
          break;
        }
        case 3: {
          // double
          E(cql_result_set_get_double_col(rs, row, col) == row,
            "expected double did not match seed data, row %d, col %d\n", row, col);
          break;
        }
        case 4: {
          // string
          // expected results:
          // s1_0  (row 0)  (s0 will be null)
          // s0_1  (row 1)
          // s1_1  (row 1)
          cql_string_ref str = cql_result_set_get_string_col(rs, row, col);
          const char *expected = row == 0 ? "s1_0" : col < 6 ? "s0_1" : "s1_1";
          E(strcmp(str->ptr, expected) == 0, "expected string did not match seed data, row %d, col %d\n", row, col);
          break;
        }
        case 5: {
          // blob
          // expected results (size, data)
          // 5, bl1_0
          // 5, bl0_1
          // 5, bl1_1
          cql_blob_ref bl = cql_result_set_get_blob_col(rs, row, col);
          const char *expected = row == 0 ? "bl1_0" : col < 6 ? "bl0_1" : "bl1_1";
          E(bl->size == 5 && memcmp(bl->ptr, expected, 5) == 0,
            "expected blob did not match seed data, row %d, col %d\n", row, col);
          break;
        }
      }
    }
  }

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_error_case_rowset(sqlite3 *db) {
  printf("Running error case rowset test\n");
  tests++;

  SQL_E(load_sparse_blobs(db));

  steps_until_fail = 5;

  get_blob_table_result_set_ref result_set;
  E(SQLITE_OK != get_blob_table_fetch_results(db, &result_set), "blob table error case failed\n");
  E(!result_set, "expected null result set for blob table");

  tests_passed++;
  return SQLITE_OK;
}

int32_t autodrop_count = 1;

cql_code test_autodrop_rowset(sqlite3 *db) {
  // this test runs twice to ensure it cleans up after itself including the tables being dropped
  printf("Running autodrop rowset test (pass %d)\n", autodrop_count++);
  tests++;

  read_three_tables_and_autodrop_result_set_ref result_set;

  SQL_E(SQLITE_OK != read_three_tables_and_autodrop_fetch_results(db, &result_set));

  for (cql_int32 i = 0; i < 3; i++) {
    int32_t id = read_three_tables_and_autodrop_get_id(result_set, i);

    E(i+1 == id, "id %d did not match %d\n", id, i+1);
  }

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_one_row_result(sqlite3 *db) {
  // this test runs twice to ensure it cleans up after itself including the tables being dropped
  printf("Running autodrop rowset test (pass %d)\n", autodrop_count++);
  tests++;

  simple_cursor_proc_result_set_ref result_set;
  simple_cursor_proc_fetch_results(&result_set);
  int32_t id = simple_cursor_proc_get_id(result_set);

  E(1 == id, "id %d did not match %d\n", id, 1);

  cql_result_set_release(result_set);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_cql_bytebuf_open(sqlite3 *db) {
  printf("Running cql_bytebuf_open test\n");
  tests++;

  cql_bytebuf b;
  cql_bytebuf_open(&b);
  int32_t max = b.max;
  int32_t used = b.used;
  cql_bytebuf_close(&b);
  E(max == BYTEBUF_GROWTH_SIZE,
    "max %d did not match expected value %d\n",
    max,
    BYTEBUF_GROWTH_SIZE);
  E(used == 0, "used %d did not match expected value %d\n", used, 0);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_cql_bytebuf_alloc_within_bytebuf_exp_growth_cap(sqlite3 *db) {
  printf("Running cql_bytebuf_alloc_within_bytebuf_exp_growth_cap test\n");
  tests++;

  cql_bytebuf b;
  cql_bytebuf_open(&b);
  int32_t init_size = b.max;
  int32_t init_used = b.used;
  int32_t needed = init_size + 10;
  cql_bytebuf_alloc(&b, needed);
  int32_t max = b.max;
  int32_t used = b.used;
  cql_bytebuf_close(&b);
  E(max == 2 * init_size + needed,
    "max %d did not match expected value %d\n",
    max,
    2 * init_size + needed);
  E(used == init_used + needed,
    "used %d did not match expected value %d\n",
    used,
    init_used + needed);

  tests_passed++;
  return SQLITE_OK;
}

cql_code test_cql_bytebuf_alloc_over_bytebuf_exp_growth_cap(sqlite3 *db) {
  printf("Running cql_bytebuf_alloc_over_bytebuf_exp_growth_cap test\n");
  tests++;

  cql_bytebuf b;
  cql_bytebuf_open(&b);
  int32_t init_size = b.max;
  int32_t init_used = b.used;
  int32_t needed = BYTEBUF_EXP_GROWTH_CAP;
  cql_bytebuf_alloc(&b, needed);
  E(b.max == needed + 2 * init_size,
    "max %d did not match expected value %d\n",
    b.max,
    needed + 2 * init_size);
  E(b.used == init_used + needed,
    "used %d did not match expected value %d\n",
    b.used,
    init_used + needed);
  init_size = b.max;
  init_used = b.used;
  needed = 2 * BYTEBUF_GROWTH_SIZE + 10;
  cql_bytebuf_alloc(&b, needed);
  int32_t max = b.max;
  int32_t used = b.used;
  cql_bytebuf_close(&b);
  E(max == init_size + needed + BYTEBUF_GROWTH_SIZE_AFTER_CAP,
    "max %d did not match expected value %d\n",
    max,
    init_size + needed + BYTEBUF_GROWTH_SIZE_AFTER_CAP);
  E(used == init_used + needed,
    "used %d did not match expected value %d\n",
    used,
    init_used + needed);

  tests_passed++;
  return SQLITE_OK;
}

cql_code some_integers_fetch(
    sqlite3 *_Nonnull _db_,
    cql_object_ref _Nullable *_Nonnull rs,
    cql_int32 start,
    cql_int32 stop) {
  return some_integers_fetch_results(_db_, (some_integers_result_set_ref _Nullable *_Nonnull)rs, start, stop);
}
