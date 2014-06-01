      SUBROUTINE MATMULKERNEL(N,M,K,A,B,C)
      IMPLICIT NONE
      INTEGER BLOCK_SIZE
      PARAMETER(BLOCK_SIZE=16)
      EXTERNAL THREADIDXX,THREADIDXY,BLOCKIDXX,BLOCKIDXY
      INTEGER THREADIDXX,THREADIDXY,BLOCKIDXX,BLOCKIDXY
      INTEGER N,M,K
      REAL A(N,K),B(K,M),C(N,M)
      INTEGER BLOCKROW,BLOCKCOL,ROW,COL,IB,I
      REAL CI
      COMMON AS,BS
      REAL AS(BLOCK_SIZE,BLOCK_SIZE),BS(BLOCK_SIZE,BLOCK_SIZE)

      BLOCKROW=BLOCKIDXX()*BLOCK_SIZE
      BLOCKCOL=BLOCKIDXY()*BLOCK_SIZE
      ROW=THREADIDXX()+1
      COL=THREADIDXY()+1
      CI=0.0
      DO 20 IB=0,K-1,BLOCK_SIZE
         AS(ROW,COL)=A(BLOCKROW+ROW,IB+COL)
         BS(ROW,COL)=B(IB+ROW,BLOCKCOL+COL)
         CALL SYNCTHREADS()
         DO 10 I=1,BLOCK_SIZE
            CI=CI+AS(ROW,I)*BS(I,COL)
 10      CONTINUE
         CALL SYNCTHREADS()
 20   CONTINUE
      C(BLOCKROW+ROW,BLOCKCOL+COL)=CI
      RETURN
      END
