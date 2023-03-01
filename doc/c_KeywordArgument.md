# About keyword argument written by C.

## case 1
  `def func1( k1:, k2: )`

```
static void c_func1(struct VM *vm, mrbc_value v[], int argc)
{
  MRBC_KW_START();
  MRBC_KW_ARG(k1, k2);
  if( !MRBC_KW_END() ) goto RETURN;

  if( !MRBC_KW_MANDATORY(k1, k2) ) goto RETURN;

  mrbc_p(&k1);
  mrbc_p(&k2);

 RETURN:
  MRBC_KW_PURGE(k1, k2);
}
```


## case 2
  `def func2( k1:, k2:, k3:"ABC" )`

```
static void c_func2(struct VM *vm, mrbc_value v[], int argc)
{
  MRBC_KW_START();
  MRBC_KW_ARG(k1, k2, k3);
  if( !MRBC_KW_END() ) goto RETURN;

  if( !MRBC_KW_MANDATORY(k1, k2) ) goto RETURN;
  if( !MRBC_KW_ISVALID(k3) ) k3 = mrbc_string_new_cstr(vm, "ABC");

  mrbc_p(&k1);
  mrbc_p(&k2);
  mrbc_p(&k3);

 RETURN:
  MRBC_KW_PURGE(k1, k2, k3);
}
```


## case 3
  `def func3( k1:, k2:, **dict )`

```
static void c_func3(struct VM *vm, mrbc_value v[], int argc)
{
  MRBC_KW_START();
  MRBC_KW_ARG(k1, k2);
  MRBC_KW_DICT(dict);

  if( !MRBC_KW_MANDATORY(k1, k2) ) goto RETURN;

  mrbc_p(&k1);
  mrbc_p(&k2);
  mrbc_p(&dict);

 RETURN:
  MRBC_KW_PURGE(k1, k2, dict);
}
```


## case of not use the macros.
  ``def func( k1:, k2: )``

```
static void c_func(struct VM *vm, mrbc_value v[], int argc)
{
  // キーワード引数があるか確認する
  if( v[argc].tt != MRBC_TT_HASH ) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "missing keyword parameter.");
    return;
  }

  // キーワード引数 k1, k2 を得る
  mrbc_value k1 = mrbc_hash_remove_by_id(&v[argc], mrbc_str_to_symid("k1"));
  mrbc_value k2 = mrbc_hash_remove_by_id(&v[argc], mrbc_str_to_symid("k2"));

  // 残りのキーワード引数があれば、エラーとする
  if( mrbc_hash_size(&v[argc]) != 0 ) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "unknown keyword parameter.");
    goto RETURN;
  }

  // k1とk2 が両方とも与えられているか確認し、与えられていなければエラーとする
  if( (k1.tt == MRBC_TT_EMPTY) || (k2.tt == MRBC_TT_EMPTY) ) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "missing keyword parameter.");
    goto RETURN;
  }

  // 表示して確認
  mrbc_p(&k1);
  mrbc_p(&k2);

  // リファレンスカウンタを採用しているので、リターン前に解放する。
 RETURN:
  mrbc_decref(&k1);
  mrbc_decref(&k2);
}
```
