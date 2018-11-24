# MAC0499 - Trabalho de formatura supervisionado

Este repositório contém o código do trabalho de formatura supervisionado de Gabriel de Russo e Carmo, disponível em https://linux.ime.usp.br/~gabrielrc/mac0499/

## Módulos

### DynamicForest

O módulo DynamicForest é uma biblioteca que resolve o problema da conecividade dinâmica em florestas. O código está documentado.

Exemplo de uso

```
#include "DynamicForest/DynamicForest.h"

int main() {
    DynamicForest df(5);
    df.link(0, 1);
    df.link(0, 2);
    df.link(3, 4);
    df.cut(0, 2);
    df.cut(3, 4);
    df.is_connected(0, 3);
}
```

### DynamicGraph

O módulo DynamicGraph é uma biblioteca que resolve o problema da conectividade dinâmica em grafos e depende do módulo DynamicForest. O código está documentado.

Exemplo de uso

```
#include "DynamicGraph/DynamicGraph.h"

int main() {
    DynamicGraph dg(4);
    dg.insert(0, 1);
    dg.insert(1, 2);
    dg.insert(2, 3);
    dg.insert(0, 3);
    dg.remove(0, 1);
    dg.remove(2, 3);
    dg.insert(1, 3);
    dg.is_connected(0, 3);
}
```

## Compilando

O código não tem nenhuma dependência externa, exceto do [Google Test](https://github.com/abseil/googletest) para testes. Assim, pode ser facilmente portado para qualquer sistema de compilação.

O sistema de compilação escolhido foi o [Bazel](https://bazel.build/). Com ele instalado, basta rodar

```
bazel build //DynamicGraph:DynamicGraph
```

para compilar a biblioteca que resolve o problema da conectividade dinâmica em grafos.

## Testes

Alguns testes de unidade estão presentes em todos os módulos e podem ser rodados com

```
bazel test //DynamicGraph:DynamicGraphTest
bazel test //DynamicForest:DynamicForestTest
```

Há também um teste exaustivo que se mostrou muito eficiente em detectar problemas durante o desenvolvimento. Pode ser executado com

```
bazel test //Stress:StressTest
```

## Perfomance

O módulo perfomance é responsável por medir o desempenho do algoritmo. Pode ser executado com

```
bazel run //Perfomance:Perfomance
```

A saída é no formato csv, contendo as informações

```
tamanho do grafo, tempo gasto com inserções, número de inserções, tempo gasto com remoções, número de remoções, tempo gasto com consultas, número de consultas
```

Todos os tempos estão em microssegundos.
