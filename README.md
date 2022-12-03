# duntil

Delay command until specific time.

## Requirements

* Linux
* C++11 compiler

## Build

```sh
$ make
```

## Usage

```sh
duntil @POSIXTIME COMMAND [ARGS..]
duntil +SECONDS COMMAND [ARGS..]
```

### @POSIXTIME

Seconds since 1970-01-01 00:00:00 UTC.
It can be a decimal number.

### +SECONDS

Seconds to wait.
It can be a decimal number.

## Example

Echo "Happy New Year!" at 2100-01-01 00:00 in localtime:

```sh
$ duntil @$(date +%s -d "2100-01-01 00:00") echo "Happy New Year!"
```

Echo "Ramen is ready." after 3 minutes:

```sh
$ duntil +180 echo "Ramen is ready."
```
