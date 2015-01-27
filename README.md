openacc compile: 
pgcc -acc Minfo=accel -o teste1 teste1.c

Texto:
- Modelo de execução de GPUs:
 1 Aloca memoria na GPU; cudaMalloc(void **d_p, size);
 2 Copia os dados pra GPU; cudaMemCopy(void *dest, void *src, CopyTo/*cudaHostToDevice*/));
 3 Faz o processamento;
 4 Copia os dados da GPU para o host e desaloca memoria da GPU; cudaMemCopy; cudaFree(void *p);

- Kernel: um grid de blocos de threads ex: __global__ kernel(params...); uma função que é chamada no host e é executada no device;
  * chamada kernel<<<NumBlocks, NumThread, shared memory, *stream>>>(params...); ex:
    dim3 blockDim(16, 16, 1);
    dim3 gridDim((width + blockDim.x - 1)/ blockDim.x, (height + blockDim.y - 1) / blockDim.y, 1);
    kernel<<< gridDim, blockDim, 0 >>>(d_data, height, width);
 
   __global__ void kernel(float* odata, int height, int width)
   {
      unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
      unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;
      if (x < width && y < height) {
         float c = tex2D(tex, x, y);
         odata[y*width+x] = c;
      }
   }
  * NumBlocks e o numero de threads podem ter grande impacto no desempenho(depende do modelo da placa); possui um modelo de dimensões; facilita o desenvolvimento pra mutidimensões; max 3 dimensões;
  * NumThread: no max 1024 thread por bloco; ideal no minimo 32 threads por bloco e multiplo de 32;
  * Cada bloco é executado em um SM;
  * Sincronização de threads só permitida entre threads de um bloco: __syncthreads();
  * Streams uma maneira de melhorar o desempenho fazendo um "pipeline" entre execuções na cpu e gpu e transfererncias de dados, fazendo a execução destes em paralelo melhorando o desempenho(https://www.youtube.com/watch?v=RGSoRSoHapY min 42) - Muito interessante.
 
- Uma gpu possui vários SMs(stream Multprocessors) - São + simples, sem pipelines complicados e sistemas de predições, unidade de controle tambem + simples:
  * SMs possuem dividem a L1 e a shared memory.
  * Executam warps - 32 threads executando a mesma instrução, cada uma pode seguir branchs diferentes, mas isso deixa a execução mais lenta, pois, as threads num warp em um branch de execução deferente tem q esperar que as outras threads terminem a execução da instrução par então executar as thread nesse branch ; utilizar #pragma unroll /*desenrola laços com constantes*/
  * As 32 threads acessam a memoria ao mesmo tempo, ou seja são carregados 32 words - para optmização, evitar acessar a mesma posição de memória, a melhor opção eh fazer com q as threads trabalhem com posições de memoria proximas umas das outras.
  * Utilizar dados read-only aumenta o desempenho: const __registry__ ; alguma parada assim.
  * Um SM pode executar warps simultaneamente dependendo do numero de threads restantes no bloco e/ou disponibilidade de registradores e shared memory(Ocuppancy, ver mais em http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#occupancy-calculator)

- Shared memory possui velocidade aproximada de um registrador, 48kb(apartir das placas com computabilidade 2.0(-arch=sm_20))).
  * A shared memory esta organizada em bancos de 32, ou seja, 32 thread podem acessar simultanemente esses bancos.
  * Evitar conflito de banco, a shared memory possui um sistema de broadcast, ou seja se tds as 32 threads acessarem a mesma posição entaum n haverá penalidade.
  * Criar um esquema de acesso diagonal da shared mem (https://www.youtube.com/watch?v=qOCUQoF_-MM min 7:50).
 
- L2 e global memory são compartilhados entre SMs.

- Evitar mover sempre dados entre cpu e gpu(leva muito tempo).
