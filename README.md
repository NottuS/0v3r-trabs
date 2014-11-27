openacc compile: 
pgcc -acc Minfo=accel -o teste1 teste1.c

Texto:
- Uma gpu eh dividida em SMs(stream Multprocessors):
  * SMs possuem dividem a L1 e a shared memory(velocidade aproximada de um registrador, 48kb).
  * Executam um warp - 32 threads executando a mesma instrução, cada uma pode seguir branchs diferentes, mas isso deixa a execução mais lenta, pois, não eh legal ter 32 threads executando instruções diferentes.
  * As 32 threads acessam a memoria ao mesmo tempo, ou seja são carregados 32 words - para optmização, evitar acessar a mesma posição de memória, a melhor opção eh fazer com q as threads trabalhem com posições de memoria proximas umas das outras.
  * Utilizar dados read-only aumenta o desempenho: const __registry__ ; alguma parada assim.
- L2 e global memory são compartilhados entre SMs
