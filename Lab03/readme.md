# Lab3 BDD (Binary Decision Diagram) by CUDD

> NYCU EE 0811562 何祁恩

## Compile:

Please type make installCUDD to install CUDD package:
```shellscript=
$	chmod +x CUDD_install.sh
$	./CUDD_install.sh
```

Please type cd src to entry the source code folder:
```bash=
$	cd src
```

Please type make to compile the source code and link to the CUDD package:
```bash=
$	make
```

## Execute:
Please type make runall to run 3 public cases:
```bash=
$	make runall
```

Please type make seeall to see the output of the 3 public cases:
```bash=
$	make seeall
```

## Visualize Graphviz
Please type the following command to visualize the BBD
```bash=
$ 	usage: dot -Tsvg [.dot filename] -o [picture name]
$	ex: dot -Tsvg 1_order.dot -o 1_order.svg
```

### case1.txt:
```bash=
ab+cd.
acbd.
abcd.
```

*	in acbd reduce order (Cudd_DagSize = 8):
![](https://i.imgur.com/mqi2JG6.png)
*	in abcd reduce order (Cudd_DagSize = 6):
![](https://i.imgur.com/EEzIMvX.png)

### case2.txt
```bash=
ab+cd+ef+gh+ij+Aa+Bb+Cc+Dd+Ee+Ff.
abcdefghij.
acegibdfhj.
```
*	in abcdefghij reduce order (Cudd_DagSize = 12):
![](https://i.imgur.com/9veNXyE.png)
*	in acegibdfhj reduce order (Cudd_DagSize = 64):
![](https://i.imgur.com/lXS5Qex.png)

### case3.txt
```bash=
a+b+c+d+e+f+g+h+i.
abcdefghi.
ihgfedcba.
acegibdfh.
```
*	in abcdefghi reduce order (Cudd_DagSize = 11):
![](https://i.imgur.com/MvfSUsn.png)
*	in ihgfedcba reduce order (Cudd_DagSize = 11):
![](https://i.imgur.com/rZ5d9We.png)
*	in acegibdfh reduce order (Cudd_DagSize = 11):
![](https://i.imgur.com/5IjsXzD.png)