# L01: Presentación del Laboratorio

Esta sesión es introductoria y de carácter demostrativo. En lugar de una actividad práctica, esta clase recorre de forma anticipada las principales herramientas que se usarán durante el semestre, trabajando sobre un programa en C que evoluciona a medida que se introducen cada una de ellas.

El objetivo no es dominar estas herramientas hoy, cada una tendrá su sesión dedicada, sino tener una primera visión del conjunto y entender para qué sirve cada pieza.

## Contexto del problema

El Imperio Galáctico ha desplegado una red de sondas en el Outer Rim en busca de bases rebeldes. Cada lote de sondas patrulla un conjunto de sistemas estelares: la mayoría no encuentra nada y continúa en órbita en silencio, pero aquellas que detectan actividad rebelde transmiten una señal de alerta (intensidad 0–100) antes de ser destruidas por los rebeldes. El sistema de inteligencia imperial debe procesar esas alertas en tiempo real.

Antes de desplegar el sistema en producción, se prueba con datos simulados el comportamiento de las sondas mediante números aleatorios: el número de sondas por lote, cuántas detectan actividad rebelde y la intensidad de cada alerta.

El programa `sondas` implementa ese sistema de monitoreo. A lo largo de la sesión se escribe, compila, automatiza, depura y perfila el programa, introduciendo una herramienta nueva en cada etapa.

## Requisitos

- Máquina virtual Lubuntu con VirtualBox
- Herramientas instaladas: `gcc`, `make`, `gdb`, `valgrind`, `htop`
- Clonar este repositorio:

```bash
git clone <url-del-repo> lab01
cd lab01
ls
pwd
```

## Actividad

### Etapa 1: Compilación manual con gcc

El compilador `gcc` transforma el código fuente `.c` en un ejecutable binario. La compilación se realiza manualmente desde la terminal.

```bash
gcc sondas.c
```

Al ejecutar el programa con `./a.out` se observa que el programa funcionando. La salida del programa es algo como esto:

```
=== Monitor de Sondas Imperiales - Sector Outer Rim ===

Lote       1 | sondas:  5 | detecciones:  3 | max:  91 | min:  43 | avg:  68.33
Lote       2 | sondas:  3 | detecciones:  2 | max:  78 | min:  55 | avg:  66.50
Floating point exception (core dumped)
```

#### Flag -Wall

Un **flag** de compilación es un modificador que se agrega al comando `gcc` para activar opciones adicionales. En este caso, el flag `-Wall` activa todas las advertencias del compilador, lo que ayuda a identificar posibles errores en el código.

```bash
gcc -Wall sondas.c
```

Con lo cual nos damos cuenta de lo siguiente:

```
sondas.c: In function ‘main’:
sondas.c:26:7: warning: unused variable ‘dummy’ [-Wunused-variable]
   26 |   int dummy = 0;
      |
```

#### Flag -o

El flag `-o` permite especificar el nombre del ejecutable de salida. En lugar de `a.out`, podemos nombrarlo `sondas`:

```bash
gcc -Wall -o sondas sondas.c
```

#### Flag -std=c11

C ha tenido una larga evolución en el tiempo, lo que ha derivado en diferentes estándares (C89, C99, C11, etc.) con distintas características. El flag `-std=c11` indica que queremos usar el estándar C11, lo que garantiza compatibilidad y acceso a las funciones más modernas del lenguaje.

```bash
gcc -Wall -std=c11 -o sondas sondas.c
```

### Etapa 2: Automatización con make

Cuando el proyecto crece, reescribir el comando `gcc` en cada compilación es tedioso y propenso a errores. `make` automatiza ese proceso a partir de un archivo llamado `Makefile`.

```bash
make          # compila
./sondas
make          # no recompila si no hubo cambios
make clean    # elimina el binario
```

El `Makefile` de esta etapa tiene la estructura mínima:

```makefile
CC     = gcc
CFLAGS = -Wall -std=c11

all: sondas

sondas: sondas.c
    $(CC) $(CFLAGS) -o sondas sondas.c

clean:
    rm -f sondas
```

`make` detecta automáticamente si el archivo fuente cambió y solo recompila cuando es necesario.

### Etapa 3: Depuración con gdb

El programa opera en modo continuo procesando lotes, pero esporádicamente se produce un crash `Floating point exception (core dumped)`. Si bien podríamos inspeccionar detalladamente el código, podemos usar `gdb` para identificar exactamente dónde ocurre el crash y qué variables están involucradas.

`gdb` permite ejecutar el programa en modo controlado y detenerlo exactamente en el momento del crash para inspeccionar el estado interno:

```bash
gdb ./sondas
(gdb) run
```

Obtenemos algo como:
```
Program received signal SIGFPE, Arithmetic exception.
0x0000555555555251 in calcular_promedio ()
```

La idea de usar `gdb` es poder obtener información detallada sobre el error, como la línea exacta del código donde ocurre, los valores de las variables en ese momento y el stack trace completo. Ante esto, podemos intentar analizar el código máquina en el punto del error mediante `disassemble`, lo que nos da algo como:

```
Dump of assembler code for function calcular_promedio:
   0x0000555555555209 <+0>:     endbr64
   0x000055555555520d <+4>:     push   %rbp
   0x000055555555520e <+5>:     mov    %rsp,%rbp
   0x0000555555555211 <+8>:     mov    %rdi,-0x18(%rbp)
   0x0000555555555215 <+12>:    mov    %esi,-0x1c(%rbp)
   0x0000555555555218 <+15>:    movl   $0x0,-0x8(%rbp)
   0x000055555555521f <+22>:    movl   $0x0,-0x4(%rbp)
   0x0000555555555226 <+29>:    jmp    0x555555555245 <calcular_promedio+60>
   0x0000555555555228 <+31>:    mov    -0x4(%rbp),%eax
   0x000055555555522b <+34>:    cltq
   0x000055555555522d <+36>:    lea    0x0(,%rax,4),%rdx
   0x0000555555555235 <+44>:    mov    -0x18(%rbp),%rax
   0x0000555555555239 <+48>:    add    %rdx,%rax
   0x000055555555523c <+51>:    mov    (%rax),%eax
   0x000055555555523e <+53>:    add    %eax,-0x8(%rbp)
   0x0000555555555241 <+56>:    addl   $0x1,-0x4(%rbp)
   0x0000555555555245 <+60>:    mov    -0x4(%rbp),%eax
   0x0000555555555248 <+63>:    cmp    -0x1c(%rbp),%eax
   0x000055555555524b <+66>:    jl     0x555555555228 <calcular_promedio+31>
   0x000055555555524d <+68>:    mov    -0x8(%rbp),%eax
   0x0000555555555250 <+71>:    cltd
=> 0x0000555555555251 <+72>:    idivl  -0x1c(%rbp)
   0x0000555555555254 <+75>:    pop    %rbp
   0x0000555555555255 <+76>:    ret
End of assembler dump.
```

Sin embargo esto es difícil de interpretar; no obstante, es posible compilar el programa de modo que `gdb` pueda mapear el código máquina a las líneas del código fuente, y así saber exactamente qué línea de `sondas.c` está causando el crash.

Para obtener símbolos de depuración, compilar con el flag `-g` que podemos agregar a `CFLAGS` en el `Makefile`. Luego de compilar, al llegar al error podemos usar varios comandos de `gdb` para inspeccionar el estado del programa, entre los que destacaremos:

- **backtrace**: Muestra el stack trace completo, es decir, la secuencia de llamadas a funciones que llevó al crash
- **list**: Muestra el código fuente alrededor de la línea actual, lo que ayuda a entender el contexto del error
- **print**: Permite inspeccionar el valor de variables específicas en el momento del error

### Etapa 4: Detección de leaks con valgrind

Con el bug anterior corregido, el programa ya no colapsa, por lo que usamos la herramienta `htop` para monitorear su funcionamiento; sin embargo, se observa que el consumo de memoria crece de forma sostenida. Esto sugiere que hay un **leak de memoria**, es decir, que el programa está asignando memoria pero no la libera adecuadamente.

Este es un error diferente: no genera que el programa se interrumpa, pero sí puede llevar a un consumo excesivo de memoria que eventualmente agote los recursos del sistema. Para detectar exactamente dónde ocurre el leak, se puede usar `valgrind`, una herramienta que analiza el uso de memoria en tiempo de ejecución y reporta cualquier asignación sin liberación correspondiente. Para usar `valgrind`, se ejecuta el programa a través de esta herramienta:

```bash
valgrind --leak-check=full ./sondas
```

Tras algunos lotes podemos terminar el programa con `Ctrl+C` y analizar el reporte que nos mostrará algo como esto:

```
==878570== 
==878570== Process terminating with default action of signal 2 (SIGINT)
==878570==    at 0x4961A7A: clock_nanosleep@@GLIBC_2.17 (clock_nanosleep.c:78)
==878570==    by 0x496EA26: nanosleep (nanosleep.c:25)
==878570==    by 0x4983C92: sleep (sleep.c:55)
==878570==    by 0x1094AB: main (sondas.c:113)
==878570== 
==878570== HEAP SUMMARY:
==878570==     in use at exit: 542,220 bytes in 24,552 blocks
==878570==   total heap usage: 24,552 allocs, 0 frees, 542,220 bytes allocated
==878570== 
==878570== 541,196 bytes in 24,551 blocks are definitely lost in loss record 2 of 2
==878570==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==878570==    by 0x109340: analizar_lote (sondas.c:81)
==878570==    by 0x1094A1: main (sondas.c:112)
==878570== 
==878570== LEAK SUMMARY:
==878570==    definitely lost: 541,196 bytes in 24,551 blocks
==878570==    indirectly lost: 0 bytes in 0 blocks
==878570==      possibly lost: 0 bytes in 0 blocks
==878570==    still reachable: 1,024 bytes in 1 blocks
==878570==         suppressed: 0 bytes in 0 blocks
==878570== Reachable blocks (those to which a pointer was found) are not shown.
==878570== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==878570== 
==878570== For lists of detected and suppressed errors, rerun with: -s
==878570== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

Con lo anterior podemos identificar exactamente en qué línea del código ocurre la solicitud de memoria que no se está liberando, analizar el contexto de esa línea y corregir el error. 

### Etapa 5: Perfilamiento con time

Con los bugs corregidos, se puede medir el tiempo de ejecución del programa. Para que la medición sea significativa se elimina el `sleep(1)` del bucle y se reemplaza el `while(1)` por un número fijo de un millón de iteraciones.

Luego se recompila y se mide utilizando el comando `time`:

```bash
make
time ./sondas
```

Salida esperada:

```
=== Monitor de Sondas Imperiales - Sector Outer Rim ===

Lote       1 | sondas:  5 | detecciones:  3 | max:  91 | min:  43 | avg:   68
Lote       2 | sondas:  3 | detecciones:  2 | max:  78 | min:  55 | avg:   66
...
Lote  999999 | sondas: 10 | detecciones:  3 | max:  81 | min:  24 | avg:   46
Lote 1000000 | sondas:  1 | detecciones:  1 | max:   0 | min:   0 | avg:    0

real    0m6.324s
user    0m0.335s
sys     0m0.694s
```

Los tres tiempos que reporta `time`:

| Campo | Significado |
|-------|-------------|
| `real` | Tiempo de reloj transcurrido (lo que siente el usuario) |
| `user` | Tiempo de CPU consumido por el propio proceso |
| `sys`  | Tiempo de CPU consumido en llamadas al sistema operativo |

## Resumen de herramientas

A lo largo de la sesión se han introducido varias herramientas fundamentales para el desarrollo de software en C. Cada una tiene un propósito específico y se utilizará en diferentes etapas del desarrollo y mantenimiento del programa. A continuación se presenta un resumen de las herramientas vistas hoy y en qué sesión se profundizará en cada una de ellas:

| Herramienta | Para qué sirve              | Sesión en detalle |
|-------------|-----------------------------|-------------------|
| `vim`       | Editar código en terminal   | L03               |
| `gcc`       | Compilar C                  | L03               |
| `make`      | Automatizar compilación     | L13               |
| `gdb`       | Depurar crashes             | L06               |
| `valgrind`  | Detectar leaks de memoria   | L07               |
| `time`      | Medir tiempo de ejecución   | L09               |
