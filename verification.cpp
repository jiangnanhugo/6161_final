/* Code directly adapted from T. Hagerup's MST Verification code written in D */

int[] tree_path_maxima(int root,int[] child,int[] sibling, double[] weight,int[] upper,int[] lower) {

  int height=0,n=child.length,m=upper.length;
  int[] depth=new int[n],D=new int[n],L=new int[n], Lnext=new int[m], answer=new int[m],median,P;
  
  void init(int u,int d) { // d = depth of u
    depth[u]=d;
    if (d>height) height=d; // height of T = maximum depth
    for (int i=L[u];i>=0;i=Lnext[i])
	D[u]|=1<<depth[upper[i]];
    for (int v=child[u];v>=0;v=sibling[v]) {
      init(v,d+1);
      D[u]|=D[v]&~(1<<d);
    }
  }
  
  // Returns a table of size 2^(h+1) whose entry in position i,
  // i=0,...,2^(h-1)-1, is the median of the set represented by int[] T=new int[(1<<h)+1],median=new int[1<<h+1];
  int[] median_table(int h) {

    // Stores the subsets of size k of {0,...,n-1} in T,
    // starting in position p, and returns p plus their number.
    int subsets(int n,int k,int p) {
      if (n<k) return p;
      if (k==0) { T[p]=0; return p+1; }
      int q=subsets(n-1,k-1,p);
      for (int i=p;i<q;i++) T[i]|=1<<(n-1);
      return subsets(n-1,k,q);
      }//end subsets
      
      for (int s=0;s<=h;s++) {
	for (int k=0;k<=s;k++) {
	  int p=subsets(h-s,k,0);
	  int q=subsets(s,k,p);
	  q=subsets(s,k+1,q);
	  for (int i=0;i<p;i++) {
	    int b=(1<<s+1)*T[i]+(1<<s); // fixed high bits
	    for (int j=p;j<q;j++)
	      median[b+T[j]]=s; // variable low bits
	  }
	}
      }
    return median;
  } // end median_table
  
  // Returns A "downarrow" B
  int down(int A,int B) {
    return B&(~(A|B)^(A+(A|~B)));
  }
  
  void visit(int v,int S) { // S = S of parent
  int binary_search(double w,int S) {
  // Returns max({j in S | weight[P[j]]>w} union {0})
  if (S==0) return 0;
  int j=median[S];
  while (S!=1<<j) { // while |S|>1
  S&=(weight[P[j]]>w)?~((1<<j)-1):(1<<j)-1;
  j=median[S];
  }
  return (weight[P[j]]>w)?j:0;
  }
  P[depth[v]]=v; // push current node on stack
  int k=binary_search(weight[v],down(D[v],S));
  S=down(D[v],S&(1<<(k+1)-1)|(1<<depth[v]));
  for (int i=L[v];i>=0;i=Lnext[i])
  answer[i]=P[median[down(1<<depth[upper[i]],S)]];
  for (int z=child[v];z>=0;z=sibling[z]) visit(z,S);
  } // end visit
  L[]=-1; Lnext[]=-1; // initialize all array elements to -1
  for (int i=0;i<m;i++) { // distribute queries to lower nodes
  Lnext[i]=L[lower[i]];
  L[lower[i]]=i;
  }
  D[]=0; // initialize all array elements to 0
  init(root,0);
  P=new int[height+1];
  median=median_table(height);
  visit(root,0);
  return answer;
} // end tree_path_maxima
