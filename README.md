
# Test case

## Index
---
[1. built-in](#built-in)<br/>
[2. shell_param_expansion](#shell_param_expansion)<br/>
[3. filename_expansion](#filename_expansion)<br/>
[4. heredoc](#heredoc)<br/>
[5. redirection](#redirection)<br/>

---
### built-in
---
```sh
# echo
echo asdf					# asdf\n
echo -n asdf asdf sadf		# asdf asdf sadf (no nl)
echo						# \n
echo -n						# (no nl)
echo -nnnnn					# (no nl)
echo -nasdf qwer			# -nasdf qwer
echo -n -n -n -n -nnnn sdf	# sdf (no nl)

# cd
cd							# move to home
unset HOME && cd			# bash: cd: HOME NOT SET
cd path						# move by CDPATH and display path
							# if none, move by absolute path

cd path/path				# move by absolute/relative path
cd -						# move by OLDPWD, display OLDPWD
cd NON						# bash: cd: No such file or directory
cd path path path			# move by first arg
cd path NON					# non error
cd NON path					# error, no dir change

# pwd
pwd							# display curr path
pwd arg						# diplay curr path, non err

# export
export						# display envp list with sorted format
							# should print all of envp including NULL valued
export a					# set a with no value
export a=1					# set a with value 1
export a=					# set a with value ""
export a=1 b=2 c=3			# set a with 1, b with 2, c with 3
export 1=a					# bash: export: `1=a': not a valid identifier
export (=d					# bash: export: `(=a': not a valid identifier
export åß=a					# bash: export: `åß=a': not a valid identifier

# unset
unset						# do nothing
unset a						# unset a
unset a=1					# bash: unset: `a=1': not a valid identifier
unset a=					# bash: unset: `a=': not a valid identifier
unset a b c					# unset a b c

# env
env							# display envp
							# should print '\0' value
							# should not print NULL value

# exit
exit						# exit with curr exit status
exit 1						# exit with 1
exit 1 2 3					# minishell: exit: too many arguments, no exit
exit a						# minishell: exit: a: numeric argument required, exit 255
exit a b c					# minishell: exit: a: numeric argument required, exit 255
exit 1 a b					# minishell: exit: too many arguments, no exit
exit a 1 2					# minishell: exit: a: numeric argument required, exit 255
exit LONG_MAX + 1L			# minishell: exit: arg: numeric argument required, exit 255
exit LONG_MIN - 1L			# minishell: exit: arg: numeric argument required, exit 255
exit -1						# exit with -1
exit +1						# exit with +1
exit -----1					# minishell: exit: arg: numeric argument required, exit 255
exit | cmd					# no exit, doesn't display exit\n (non interactive)
```
---
### shell_param_expansion
---
```sh
#USER=jaham
echo $USER					# jaham
echo "$USER"				# jaham
echo '$USER'				# $USER
echo $NONE					# NULL token (same with echo_with_no_arg)
echo $USER-asdf				# jaham-asdf
echo $USERasdf				# NULL token
echo $USER1					# NULL token

export test='""'asdf		# value is ""asdf
echo $test					# ""asdf (no quote removal)
echo "$test"				# ""asdf (remove only outer quote)

export test="  1  2   3 "	# word splitting, value is `  1  2   3 '
./argccnt $test				# 4
./argccnt "$test"			# 2 (no split in quote)

export test=" a "			# value is ` a '
echo " $test " | cat -e		# `  a  $' (insert space in quote)
echo "  "$test | cat -e		# `   a $'

export test=				# value is \0
./argccnt $test				# 1, word spitted (same with ./argccnt_with_no_arg)

export test=*				# bash performs filename expansion after shell param
echo $test					# display all files in directory (wildcard expanded)
# NOT SUPPORTED
echo $"USER"				# display USER, fixed in bash 5.1 (display $USER)
```
---
### filename_expansion
---
```sh
# export test=*
echo *
echo ************
echo a********
echo **********b
echo *d*D*D*D*D*D*D*D*D*
echo ds**DFFD***SDFDD"*"
echo D$test*a
```
---
### heredoc
---
```sh
<<eof << eof << eof			# execute heredoc three times
<< eof |||					# syntax err, execute heredoc, end with exit status 258
<< |||						# syntax err
<< 'eof'					# limit string : eof
> $USER						# no expand

# USER=jaham
<< eof						# limit string : eof
> $USER						# jaham
> "$USER"					# "jaham" (no quote removal)
> '$USER'					# 'jaham'
> $"USER"					# $"USER" (no expand single '$')
> $US"ER					# "ER
> $USERA					# \n
> "$USER-RA"				# "jaham-RA"

<< $USER					# limit string : $USER
```
---
### redirection
---
```sh
false > a && true > b				# create a only
true > a > b > c > d				# create a, b, c, d use d only
true > a < b > c					# create a, c use b for in, c for out
true >> a > b < c << d				# create a, b, use d for in, b for out

((echo a | echo b) > outfile1 && (echo c | echo d) > outfile2) > outfile3 | pwd
# create outfile1, outfile3, exec (echo a | echo b) > outfile1
# no execution after AND (broken pipe)

export test=" as   df   "
true > $test						# minishell: $test: ambigous redirect
true1 > a | false > b | true2 > c	# create a, b, c, exec true1, true 2

```
