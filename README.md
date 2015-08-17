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
 * pitched memory: Trabalha com a memoria alinhada, ou seja, quando aloca-se memoria e a quantidade de bytes não é multipla de 128, ele aloca o numero + prox multiplo de 128;
     size_t pitch;
     float* myArray;
     cudaMallocPitch(&myArray,&pitch,100,100);
   O pitch é esse valor que foi excedido, e ele deve ser levado em consideração para acesso e copias de memoria; 
            float next_column_element = myArray[(j+1)*pitch+i];
            cudaMemcpy2D(host_memory,100*sizeof(float)/*destination pitch*/,myArray,pitch,
            100*sizeof(float)/*width*/,100/*heigth*/,cudaMemcpyDeviceToHost);
    PROBLEMA: MAX DE MEM ALOCAVEL COM ISSO: 2GB  
- Evitar mover sempre dados entre cpu e gpu(leva muito tempo).
- gprof: ferramente pra analisar desempenho; gcc -O2 -g -pg myprog.c gprof ./a.out > profile.txt
- Error Correcting Code (ECC)|: Detecta e corrigi se há um erro em um bit unico da memoria; diminui o desempenho e ocupa cerca de 12% da memoria;
-  Single-Error Correct Double-Error Detect (SECDED) ECC: Detecta e corrigi erros em mult-bits da memoria.
-  Regular trafego de memoria
    * Determinar um numero maximo de registradores(-maxrregcount) por thread ou o numero de minimo de threads(_launch_bounds__); pode diminuir a ocupação e a eficiencia da execução, mas tambem pode diminuir o numero de acessos a local memory;
    * Tentar utilizar non-caching loads para mem global;
    * Aumentar o tamanho da memoria local;
    * Profile counters: l1_local_load_hit, l1_local_load_miss, l1_local_store_hit, l1_store_load_miss; incremente em 1 para cada transação de 128 bytes;
 - The number of units constrains only the number of instructions issued each cycle. I.e. each clock cycle 32 read instructions could be issued, and 32 results could be returned.

 - One instruction can read/write up to 128 bytes, so if each thread in warp reads 4 bytes and they are coalesced, then whole warp would require a single load/store instruction. If accesses are uncoalesced, then more instruction should be issued.

 - Moreover, units are pipelined, meaning multiple read/store request could be executing concurrently by single unit.
 - SFUs: The HW implementation is based on quadratic interpolation in ROM tables using fixed-point arithmetic.
 - When you call cos(), exp, sin, log,etc... you do not use the SFU, but instead perform several FMAD instructions that implement a more precise approximation of the trascendental function.

- Fermi 
 * 16 SMs
 * 1.5 TF
 * 6GB DDR5
 * 768 kb L2
 * DRAM(global): 192BG/s
 * PCI - express 2 (8GB/s)
 * SMs spcs:
   * 32 CUDA cores
   * 16 LD/ST units (load/store)
   * 4 SFU 
   * dual warp scheduler
   * 64 KB L1 cache/ share mem
   *  32k 32 bits regs

-Kepler
  * 15 SMXs
  * 1536kb L2 (GK110)
  * PCi Express 3 (15GB/s) 
  * Hyper Q - 32 HW queues GPU Direct, permite que mais CPUs cores(ate 32 ao mesmo tempo) executem tarefas na GPU.
  * Direct memcopies
  * Dynamic parallelism ,nested kernel launches, kernels and streams can generate more streams.
  * SMs Spcs:
    * clock menor q a da fermi
    * 192 CUDA cores
    * 64 DP units(double prec)
    * 32 load/store units
    * 32 SFUs
    * Quad warp scheduler, allowing four warps to be issued and executed concurrently, and two independent instructions per
warp can be dispatched each cycle. Unlike Fermi, which did not permit double precision instructions to
be paired with other instructions, Kepler GK110 allows double precision instructions to be paired with
other instructions.
    * 64kb L1/ shared mem GK110, GK210 128 kb

    * 65 kb 32-bits regs GK110, 128 kb 32-bits regs GK210
    * 48kb read-only data.
  
-Maxwell
   * 16 SMM
   * PCi Express 3 (15GB/s) 
   * 2048 Kb L2(GM204)
   * More active thread blocks per SMM(32)
   * Shared memory atomics
   * SMM spcs:
     * 128 CUDA cores
     * 32 load/store units
     * 32 SFUs
     * 96 Kb dedicated shared memory
     * 64 kb L1/Texture cache
     * Quad warp scheduler
     * 65 kb 32-bits regs GK110, 
-------------------------------------------------------------------------------------------------------------------
Robotica - SLAM
- Robotica é a ciência que compreende e manipula o mundo fisíco atraves de um dispositivo controlado pro um computador(Probabilistics robotics)

-Coordenada Homogeneas para distancia euclidiana; sistema de coordenadas utilizadas em projeção geometrica.
* Um espaço N-dimensional pode ser representado em N+1 dimensão, 4d para modelar o espaço 3d 
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
       p(Zt| Xt, m) = produtorio de i = 1 ate k (p(Z_ti | Xt, m))
 * Modelo Beam-Endpoint 
   + Facil processamento
 
 * Modelo Ray-cast
    + Considera o primeiro obstáculo na linha de visão
    + Mistura de 4 modelos: modelo gaussiano para a presença do obstáculo; 
    + Processamento n muito eficiente
   
 *  Modelo para observar landmarks com sensores range-bearing


- Extended Kalman Filter(EKF)
 * Distribuição Gaussiana
 * Marginalização: 
    + u: media;E matrix de covariancia, que indica a incerteza do sistema;
    + dado p(x) = p(x_a, x_b) = N(u, E); com u = u_a; E = E_aa   E_ab
                                                 u_b      E_ba   E_bb
   
    + A distribuição marginal eh p(x_a) = integral (p(x_a, x_b)) Dx_b = N(u, E); com u = u_a e E = E_aa
 
 * Kalman filter assume um modelo de transição e observação linear:
    x_t = A_t*x_t-1 + B_t*u_t + e_t
    z_t = C_t * x_t + sigma_t
 * Componentes do kalman filter  
    A_t : Matrix(n*m) que descreve como o estado evolui de t-1 para t sem controle/instruções ou noise.

    B_t : Matrix(n*l) que descreve como o controle u_t muda o estado de t-1 para t.
    
    C_t : Matrix(k*n) que descreve como o mapear o estado x_t para a observação z_t; o que espero encontrar em uma observação em t;
   
    e_t e sigma_t : Variavel randomica representando o processo e a medida do noise que assume-se ser independente e normalmente distribuido com covariância R_t e Q_t respectivamente.

* Movimeto em relação ao noise gaussiano:
   + p(x_t, u_t, x_t-1) = det(2*pi*R_t)^-1/2 * exp(-1/2*(x_t - A*x_t1 - B_t * u_t)^T * (R_t)^-1 * (x_t - A*x_t1 - B_t) * u_t))

   + R_t : descreve o noise do movimento
   + p(z_t|x_t) = det(2*pi*Q_t)^⁻1/2 * exp(-1/2 *(z_t - C_t*x_t)^T * (Q_t)^-1 * (z_t - C_t*x_t)) 
   + Q_t : descreve a medida do noise
 
* Dado um belief gaussiano, o belief é sempre gaussiano

* Kalman_filter(media_t-1, E_t-1, u_t, z_t)
       // E: matriz de covariância;
       // prediction step
       media'_t = A_t * media_t-1 + B_t * u_t;
       E'_t = A_t * E_t-1 * (A_t)^T + R_t;
      
       /K_t : kalman gain
       // correction step
       K_t = E'_t * (C_t)^T * (C_t * E'_t * (C_t)^T + Q_t)^-1;
       media_t =  media'_t + K_t(z_t - C_t * media'_t);
       E_t = (I - K_t*C_t)*E'_t;
       return media_t, E_t;

* Se houve duas distribuições gaussianas, uma como muita incerteza e a outra com muita certeza, a multiplicação das duas resulta numa nova gaussiana mais proxima à com maior certeza.
* Kalman gain :  indica o quanto estou certo sobre as observações em relação a meu movimento.
* O Kalman filter te da a média entre o valor predito e o observado.
* A maioria dos problemas em robotica envolvem funções não lineares(ex: trajetorias circulares), o q resulta em funções n gaussianas e impossibilite a utilização do KF. 
   x_t = g(u_t, x_t-1) + e_T
   z_t = h(X_t) + sigma_t
* O q pode ser feito é fazer uma linearização local(parte delas, pense no gráfico) dessas funções, oq o Extended Kalman Filter faz! se as não linearidades são altas então podem ocorrer divergencias.
* A linearização é feito via expanção de taylor utilizando a matrix jacobiana; que dado um vetor de funções e uma matrix (n*m) onde cada elemento da matrix é a derivada parsial de uma variavel em uma determinada dimensão:
   f(x) = f_1(x)
          f_2(x)
           :
           .
          f_m(x)

 jacobiano = f'_x1()   f'_x2()   ...  f'_xn()
             f''_x1()  f''_x2()  ...  f''_xn()
             f^m_x1()  f^m_x2()  ...  f^m_xn()
   
   g() : vetor de funções
   G_t: jacobiano de g
   Predição:
   p(x_t|u_t, x_t-1) = det(2*pi*Rt)^-1/2 * exp(-1/2 *(x_t - g(u_t, media_t-1) - G_t* (x_t-1 - media_t-1))^T * (R_t)^-1 *(x_t - g(u_t, media_t-1) - G_t * (x_t-1 - media_t-1)))
   
   h(): vetor de funções
   H_t: jacobiano de h
   Correção: 
   p(z_t, x_t) = det(2*pi*Rt)^-1/2 * exp(-1/2 * (z_t - h(media'_t) - H_t(x_t - media'_t))^T * (Q_t)^-1 * (z_t - h(media'_t) - H_t(x_t - media'_t)))
   
 * Extended Kalman_filter(media_t-1, E_t-1, u_t, z_t)
       // E: matriz de covariância; T: transposta
       // prediction step
       media'_t = g(u_t, media_t)
       E'_t = G_t * E_t-1 * (G_t)^T + R_t;
      
       /K_t : kalman gain
       // correction step
       K_t = E'_t * (H_t)^T * (H_t * E'_t * (H_t)^T + Q_t)^-1;
       media_t =  media'_t + K_t * (z_t - h(media'_t));
       E_t = (I - K_t * H_t) * E'_t;

       return media_t, E_t;

 * O EKF lineariza todas as funções não lineares para calcular o KF, onde g e h são vetores com funções locais da trajetoria não linear do robô.
 
- EKF SLAM (online: apenas calcula a posição atual do robô não todo o trajeto)
 * Estimar a posição do robô e a localização dos landmarks do ambiente.
 * Estado do sistema(plano 2D): x_t = ((x,y,teta) = pose, (m_(1,x), m_(1,y)) = lanmark1, ..., (m_(1,x), m_(n,y)) = landmark n)^T.
 * Representação do estado(media e covariancia)(ver https://www.youtube.com/watch?v=XeWG5D71gC0&index=6&list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN_, min = 13:32);
 * EKF SLAM: ciclo do filtro: Predição do estado; Medição da predição, medição, relacionar dados, atualizar.
   + Normalizar os componentes angulares.
 * Loop closing: reconhecer uma area previamente mapeada.
  + Problemas: Relacionamento de dados sobre alta ambiguidade, e possiveis ambientes simétricos.
  + Apos um Loop closing as incertezas das posições dos landmarks e da pose do robô colapsam(Diminuem muito).
  + Loop closing errados(ex achar que esta num loop closing) podem causar divergencias.
 * As incertezas em relação aos landmarks tendem a diminuir com o tempo(varias re-observações);


- Grid Map
 * O Mapa do ambiente é dividido em celulas;cada celulas contem a prob de estarem ocupadas
 * Assume-se q o ambiente n mude;
 * Dado um mapa m, celulas m_i; a distribuiçâo de probabilidade do mapa é dado por: p(m) = produtorio_i(p(m_i)); isso se deve ao fato de que as probabilidades de cada celula é independente.
 * Dado as informações dos sensores z_1:t e as poses x_1:t o mapa pode ser estimado, utilizando o Bayes filter:
    p(m|z_1:t, x_1:t) = produtorio_i(p(m_i, z_1:t, x_1:t))
   +  Não a necessidade de calcular a etapa de predição, pois o ambiente é estático
   +  Static State Binary Bayes Filter
   +  

- Particle Filters SLAM
  * Ele scala muito bem para um grande numero de landmark, é robusto quanto a ambiguidade na associação de dados.
  * Utiliza o filtro de particula para representar a distribuição de probalildade da posição do robô;
  * Bom com distribuições arbitrárias (mas deve haver uma distribuição alvo e uma distribuição proposta, utilizada para geras uma nova reamostragem, n ficou claro como isso funciona :( ).
  * Usa multiplas amostras para representar distribuiçôes arbitrárias; as amostras tem peso (onde é levado em consideração as diferenças entre a distribuição proposta e a alvo, tbm n ficou claro) de acordo com a verosemelhança da observação feita.
  * Deve haver um modelo de movimentação(distribuição proposta).
  
  * Cada particula(amostra) representa uma hipotetica pose do robô.
  
  * Assume que o robô inicialmente esta numa pose aleatóra e a medida que se movimenta(passo de predição) e vai fazendo observações do ambiente(reamostragem, passo de correção), as particulas tendem a se agrupar, assim melhorando a estimativa de onde o robô está.
  
  * Etapas do Particle Filters SLAM:
    1. Realiza-se a amostragem das particulas de acordo com o modelo proposto: x_t[j] = p(x_t|...);
    2. Pesa a importancia das amostras: w_t[j] = target(x_t[j])/ proposta(x_t[j]);
    3. Reamostragem: Representa a amostra i com proabilidade w_t[i] e repete j vezes;
  
  * Localização de Monte Carlo(MCL) = particle filter.
  
  * Otima tecnica para ser utilizada nos dias atuais para robôs móveis para low-dimensional spaces.
  
  * feature-based SLAM  
   + sample = x = (x1:t, m_1,x, m_1,y, ..., m_M,x,m_M,y); o numero de elementos representa a dimensão do problema.   
  * Rao-Blackwillization -> mapeamento
   + Modela o caminho do robô por amostragem e calcula/localiza os landmarks dada as poses do robô;
   + Como cada amostra indica que ela sabe aonde o robo está a posição dos landmarks podem se calculasd individualmente, e cada particula calcula um min EKF:
      P(x_x0:t, m_1:M | z_1:t, u1:t) = P(x_0:t| z_1:t, u_1:t) * produtorio_i=1 ate M(P(m_i| x_0:t, z:t))
                                                  Î                                            Î
                                   particle filter parecido com o MCL                        2d EKFs
   + Cada amostra é um caminho hipotético, pose inicial = (0,0,0);
   + Não há necessidade de manter poses passadas, pois nessa tecnica só interessa a prox pose;

 * FastSLAM
    + Cada landmarl é representado por um 2x2 EKF
    + Cada particula mantem M EKFs individuais:
      particula1 = [(x,y,0), Landmark1,..., LandmarkM]
    + Ou seja cada particula tem seu mapa
    + Etapas:
       1.Cada particula extende o seu caminho extarindo uma amostra da nova pose do robo
       2.O peso das particulas(Q: matrix de covariancia das medições(leva em consideração a inceteza do proprio landmark e da observação ); z' observação esperada):
         w[k] = |2*pi*Q|^-1/2 * exp{-1/2*(z_t - z'[k])^T * Q^-1*(z_t - z'[k])}
       3. Atualiza o belief dos landmarks observados(EKF update)
       4. Faz a reamostragem
   
    + Distribuição alvo: p(x_1:t | z_1:t, u_1:t)
    + Distribuição proposta: p(x_1:t | z_1:t-1, u_1:t); não leva em consideração a ultima observação, apenas utiliza a odometria
    + w[k] = integral(p(z_t|x_t[k], m_j) * p(m_j|x_1:t-1[k], z_1:t-1)dm_j); faz uma magica pq a primeira parte é igual a o calculo da covariancia do EKF e a segunda parte e a s soma d noise; oq resulta na equação de peso ali em cima.
  
  * Problema da Associação de Dados; Qual observação pertence a qual landmark?
    + As associações dependem das poses do robô;
    + Seleciona o landmark com o mais provavel "match";
    + Se o landmark observado tem baixa probabilidade de associação(a partir de um certo limiar), adciona-o como um novo landmark;
    + FastSLAM é melhor que o EKFSLAM para fazer associação de landmarks, pois é muito simples fazer associalções de dados, ja que cada amostra tem seu proprio belief, cada amostra tem sua propria associação de dados, ou seja ao inves de fazer uma so complexa associação de dados, no FastSLAM há M associações simples;
    + Pode-se fazer um esquema de arvore/busca binária para associação de landmarks (complexidade diminui de ONM para ONlogM).
    + Há a possibilidade de os landmarks serem compartilhados entre as particulas diminuindo a utilização de memória.
    + Reamostragem seletiva: Reamostragemn é necessaria para conseguir convergencia.
      * Reamostragem é arriscado, pois amostras importantes podem se perder.
      * Reamostragem so faz sentido se os pesos das particulas diferem significativamente.
      * Não faz remaostragem se a probabilidade se aproxima da distribuição alvo.
      * Particulas com peso com um certo limiar são eliminadas.
  
- Graph-based SLAM -> Mais utilizado hoje
  * Least Squares Approach
    + Abordagem utilizada para resolver sistemas sobredeterminados(sobredeterminados = Normalmente aplica-se a definição sobre o resultado de um evento (ou o resultado pode ser o próprio evento) subordinado a muitos outros, ou consequência destes. Desta forma, não há condições de se avaliar o início e o fim do evento, já que ele depende da interação com outros processos).
    + Minimizar a soma dos quadrados do erros nas equações.
    + Objetivo estimar o estado X que melhor explica as observações Z_1:n, que minimiza o erro dado todas as observações.
    + O erro e_i é a diferença entre o estado predito e o observado : e_i(X) = z_i - f_i(X).
    + Assume-se que as funções do erro são suaves na vizinhansa, então pode-se fazer linearizações locais iterativas.
    + Etapas da linearização:
       1. Calcula a primeira derivada do quadrado da função de erro.
       2. Iguala a zero e resolve o sistema linear.
       3. Obtem o novo estado do sistema.
       4. itera esse procedimento até convergir.
     + Com a linearização passada podemos corrigir X, executando as minimizações no incremento de deltax;
       e_ij(x+deltax) = e_ij(x) = J_ij * deltax
       J_ij = jacobiano = derivada e_ij(x)/ derivada em x
   * Graph-based(pq do nome): cada pose do robo é ligado por arestas formando um grafo, tambem cria-se arestas com poses ja "visitadas" pelo robô, atraves desses relacionamentos e da odometria do robô é possivel fazer a localização do robô(por ex: num caminho do robô essas arestas podem indicar o quanto ele esta distante de uma certa pose).
   
   * Cada pose do robo com dados de observações feitas naquela pose, representam um nodo do grafo;
   
   * Cada aresta representa algum relacionamento espacial entre os nodos.
   
   * Constroi-se um grafo e encontra um configuração de nodos que minimiza o erro introduzido pelas medições, assim gerando um mapa mais preciso corrigindo os nodos
   * O grafo consiste de n nodos x = x_1:n, onde cada x_i e uma transformação 2d ou 3d. Essas transformações podem ser expressa utilizando coordenada homogeneas.
   * Uma aresta é criada quando o robô move de x_i para x_i+1, onde essa aresta corresponde a informação da odometria. Quando o robô faz uma observação na mesma parte do ambiente de x_i e de x_j, uma aresta tambem é criada entre esses nodos.
  * Calcular o vetor coefficiente b e a matrix H, onde Omega é a matrix de informação(observações):
      b^T = somatorio_ij(b_ij)^T = somatorio_ij (e_ij)^T * Omega_ij * J_ij
      H = somatorio_ij(H_ij) = somatorio_ij (J_ij)^T * Omega_ij * J_ij
    + o jacobiano resultara em um matrix onde exceto as colunas x_i e x_j serão zeradas.
    + A estrutura esparsa de J_ij resultará em uma estrutura esparsa de H.
    + Essa estrutura reflete a matriz de adjacencia do grafo.
  * Construir o sistema linear:
   1. Computar o erro e_ij = t2v((Zij)^-1 * ((X_i)^-1 * X_j));
   2. Computar os blocos de jacobianos:
        A_ij = derivada e(x_i, x_j)/ derivada em x_i
        B_ij = derivada e(x_i, x_j)/ derivada em x_j
   3. Atualizar o vetor coefficiente:
       (b_i)^T += (e_ij)^T * Omega_ij * A_ij
       (b_j)^T += (e_ij)^T * Omega_ij * B_ij
   4. Atualiza o sistema de matrix
       H_ii += (A_ij)^T * Omega_ij * A_ij
       H_ij += (A_ij)^T * Omega_ij * B_ij
       H_ji += (B_ij)^T * Omega_ij * A_ij
       H_jj += (B_ij)^T * Omega_ij * B_ij
* Algoritmo:
    while(!converged) {
       (H,b) = buildLinearSystem(x);
       deltax = solveSparse(Hdeltax = -b)
       x = x + deltax
    }
    return x;
* O sistema pode n haver resultado, para reslver isso pode-se adcionar algum tipo de restrição.

* Com o tempo a matriz de covariancia pode ficar enorme, ficando dificil o calculo em tempo real, por isso é interessante fazer buscas locais no grafo, ou seja, delimitar a area onde robo possa estar ao inves de buscar em todo o grafo, assim corrigindo apenas aqla area local.

* Hierarquico pose-graph:
    + O grafo e divido em grupos dependendo da conectividade dos nodos;
    + Um nodo passa a representar o grupo(Alto nivel);
    + Se o houver incosistencias a correção é propaganda para os niveis menores;
    + As aresta entre os nodos de alto nive podem ser representadas pelas distancia entre os nodos de alto nivel;

* Graph-Slam com landmarks
   + Nodos podem ser poses do ou a posiçao de um landmark;
   + Arestas podem ser informaçoes do odometria ou a informação de observação do landmarl
   + A minimização otimiza a localização dos landmarks e das poses do robo
      x_i = robot
      x_j = landmark
      teta_i = orientação do robo
      z'_ij(x_i, x_j) = atan((x_j - t_i)*y/((x_j - t_i)*x) - teta_i
     e_ij = atan((x_j - t_i)*y/((x_j - t_i)*x) - teta_i - z_j
