openacc compile: 
pgcc -acc Minfo=accel -o teste1 teste1.c

Texto:
GPU - CUDA

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
  * Um SM pode executar warps simultaneamente dependendo do numero de threads restantes no bloco e/ou disponibilidade de registradores e shared memory(Ocuppancy, ver mais em http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#occupancy-calculator), codigo pra checar ocupação:
 int main() { 
    int numBlocks; // Occupancy in terms of active blocks 
    int blockSize = 32; 
    // These variables are used to convert occupancy to warps int device; 
    cudaDeviceProp prop; 
    int activeWarps; 
    int maxWarps; cudaGetDevice(&device); 
    cudaGetDeviceProperties(&prop, device); 
    cudaOccupancyMaxActiveBlocksPerMultiprocessor( &numBlocks, MyKernel, blockSize, 0); 
    activeWarps = numBlocks * blockSize / prop.warpSize; 
    maxWarps = prop.maxThreadsPerMultiProcessor / prop.warpSize; 
    std::cout << "Occupancy: " << (double)activeWarps / maxWarps * 100 << "%" << std::endl; 
    return 0;
}


- Shared memory possui velocidade aproximada de um registrador, 48kb(apartir das placas com computabilidade 2.0(-arch=sm_20))).
  * A shared memory esta organizada em bancos de 32, ou seja, 32 thread podem acessar simultanemente esses bancos.
  * Evitar conflito de banco, a shared memory possui um sistema de broadcast, ou seja se tds as 32 threads acessarem a mesma posição entaum n haverá penalidade.
  * Criar um esquema de acesso diagonal da shared mem (https://www.youtube.com/watch?v=qOCUQoF_-MM min 7:50).
 
- L2 e global memory são compartilhados entre SMs.
 * Acessar a mem em pos multiplas de 32, e de maneira unificada onde tds as threads acessem pos "seguidas" 
 * Evitar acessar a memória de forma desalinhada, permutada ou q so algumas threads carreguem dados; o hardware serializa o acesso nesses casos.

- Evitar mover sempre dados entre cpu e gpu(leva muito tempo).
- gprof: ferramente pra analisar desempenho; gcc -O2 -g -pg myprog.c gprof ./a.out > profile.txt

-------------------------------------------------------------------------------------------------------------------
Robotica - SLAM

-Coordenada Homogeneas para distancia euclidiana; sistema de coordenadas utilizadas em projeção geometrica.
 * 3d - 2d; para um ponto X
 
X = matrix u = u/w -> u/w = x
           v   v/w    v/w   y
           w    1
 * Um ponto no infinito pode ser representado como:
 Xinf = matrix u
               v
               0
* 4d - 3d
X = matrix u = u/t -> u/t
           v   v/t    v/t
           w   w/t    w/t
           t    1

-Qualquer tranformação em coordenada homogeneas pode ser expressa por:
 x' = Mx
 
-Translação
 M = scala * matrix I   t
                    0^p 1
                    
 scala = depende da scala
 p = dimensão
 I = matrix 1 0 0   t = matrix tx
            0 1 0              ty
            0 0 1              tz
            
 - Rotação
 * M = scala * matrix R    0
                      0^p  1
 * 2d 
 R(teta) = matrix cos(teta)  -sen(teta)
                  sen(teta)   cos(teta)
 * 3d 
 rotação no eixo x
 Rx(w) = matrix 1   0       0
                0 cos(w) -sen(w)
                0 sen(w)  cos(w)

rotação no eixo y
 Ry(l) = matrix cos(l) 0  sen(l)
                 0     1    0
               -sen(l) 0  cos(l)
 
 rotação no eixo z              
 Rz(k) = matrix cos(k) -sen(k) 0
                sen(k) cos(k)  0
                 0       0     1
 qualquer eixo:                
 R(w,l,k) = Rz(k)Ry(l)Rx(w)
 
 - Movimento = Rotação + Translação
 * M = scalar * matrix R    t
                       0^p  1

 - Inverte Transformações 
  x = M^-1 * x'

- Estimativa da posição (Bayes Filter): estimar o estado x do sistema dado as observações z e um conjunto de instruções u em um mapa m.
 * p(x|z,u)
 * belief: prob de um certo estado do sistema em um tempo t; 
 * bel(Xt) = p(Xt| Z1:t, U1:t) = N * p(Zt|Xt) * Integral xt - 1 (p(Xt|Xt-1, Ut) * bel(Xt-1)) DXt-1 
 * Bayes filter pode ser descrito em duas etapas:
  + Predição : bel'(Xt) = integral (p(Xt|Ut,Xt-1) * bel(Xt-1)) DXt-1; a partir do deslocamento(modelo) do robô; estima-se qual sua atual posição.
  + Correção : bel(Xt) = n p(Zt|Xt) * bel'(Xt); a partir das observações(modelo) do ambiente o belief é ajustado ou corrigido.
 
- Modelo de Movimentação do Robô: Robo se move de (x,y,teta) para (x', y', teta');
* Os dois tipo + utilizados são:
1 - Baseado em odometria; + preciso
 + Geralmente utilizados quando se tem encoders nas rodas;
       u = (rot1, trans, rot2)
       trans = raiz((x' - x)^2 + (y' - y)^2)
       rot1 = atan2(y' - y, x' - x) - teta
       rot2 = teta1 - teta - rot1

2 -  Baseado em velocidade
 + Geralmente utilizados quando não se tem encoders nas rodas
 + Se o robo se move em circulo, o circulo(caminho) contem a orientação final, por isso há a necessidade de adcionar um certo erro na orientação final: gama * delta t;
  u = (v, w)
  
  x'      x        -v/w * sen(teta) + v/w * sen(teta + w * deltat)
  y' =    y    +   -v/w * cos(teta) + v/w * cos(teta + w * deltat)
 teta   teta              w * delta t + gama * delta t

- Modelo de Observação
  * Modelo para laser scanner
    + O scan consiste de K medidas; ;Cada medida é independe da posição do robô
       Zt = {Zt1,Zt2, ..., Ztk}
       p(Zt| Xt, m) = produtorio de i = 1 ate k (p(Zti | Xt, m))
 * Modelo Beam-Endpoint 
   + Facil processamento
 
 * Modelo Ray-cast
    + Considera o primeiro obstáculo na linha de visão
    + Mistura de 4 modelos: modelo gaussiano para a presença do obstáculo
   
