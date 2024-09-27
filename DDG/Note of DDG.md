> Discrete Differential Geometry : An Applied Introduction, by Keenan Crane
> [Keenan Crane - Discrete Differential Geometry (cmu.edu)](https://www.cs.cmu.edu/~kmcrane/Projects/DDG/)
# 2. Combinatorial Surface
#### **Combinatorial Surface**
- Descriptions of shapes that only tell you how surfaces are **connected up** and not **where they are in space**. 
-  In discrete differential geometry, combinatorial surfaces play the **same role** that **Topological Surfaces** do in the smooth setting.
#### **Manifold**
- Loosely speaking this means that, at least under a **microscope**, they look the same as ordinary **Euclidean space**.
#### **Topological Disk**
- is, roughly speaking, any shape you can get by deforming the unit disk in the plane **without** *tearing* it, *puncturing* it, or *gluing* its edges together.
## 2.1 Abstract Simplicial Complex
> Vertex/Vertices  Simplex/Simplices  Complex/Complexes
#### **Abstract Simplicial Complex**
- specifies how vertices are **connected**, but **not where they are in space**.
#### **Simplicial Complex**
- **V**: A set of vertices $V=\{0,1,2,\dots,n\}$
- **k-simplex**: Subsets of **V**, which is a set of **k** distinct **vertices**.
	- **0-simplex** - Vertex
	- **1-simplex** - Edge
	- **2-simplex** - Triangle
- **Face**: Any **nonempty subset** of a simplex
- **Proper face**: Face with **strict** subset
- **Simplicial Complex**($\mathcal{K}$):  A collection of simplices. For every simplex $\sigma\in\mathcal{K}$, every **face** $\sigma'\subseteq\sigma$ is also contained in $\mathcal{K}$.
- **Subcomplex**($\mathcal{K}'$) of a simplicial complex($\mathcal{K}$): A subset that is also a simplicial complex.
- **Pure k-simplicial complex**: A complex($\mathcal{K}$). every simplex $\sigma'\in\mathcal{K}$ is contained in some **simplex of degree k** (possibly itself).
	- So to speak, no simplex with k' less than k hanging off.
## 2.2 Anatomy of a Simplicial Complex: Star, Closure, and Link
#### **Star**
> for any subset $\mathcal S$ of $\mathcal K$ (*not necessarily a subcomplex* ) , i.e. $St(\mathcal S)$
- The collection of all simplices $\color{#8E7}{\sigma\in\mathcal{K}}$ such that for $\forall \sigma' \in \mathcal{S}, \color{#8E7}{\exists \sigma' \in\sigma}$.
- May no be a **simplicial complex**, since there is no "boundary".
#### **Closure**
> for any subset $\mathcal S$ of $\mathcal K$ (not necessarily a subcomplex) , i.e. $Cl(\mathcal S)$
- The **smallest subcomplex** of $\mathcal{K}$ containing $\mathcal S$.
- A closure of a vertex is itself.
#### **Link**
> for any subset $\mathcal S$ of $\mathcal K$ (not necessarily a subcomplex) , i.e. $Lk(\mathcal S)$
- $\color{#8E7}{Lk(\mathcal{S})=Cl(St(\mathcal{S}))\backslash St(Cl(\mathcal{S}))}$
- Or the **ring**.
#### **Boundary**
> for a **pure k-subcomplex** $\mathcal{K}' \subseteq \mathcal{K}$, i.e. $Bd(\mathcal{K}')$
- Is the **closure** of the set of all simplices $\sigma$ that are **proper faces** of **exactly one** simplex of $\mathcal{K}'$.
#### **Interior**
> for a **pure k-subcomplex** $\mathcal{K}' \subseteq \mathcal{K}$, i.e. $Int(\mathcal{K}')$
- $Int(\mathcal{K}')=\mathcal{K}\backslash Bd(\mathcal{K}')$.
#### **Even/Odd Permutation**
- Permutations with even/odd number of **inversions** they contain.
- **Inversion**: A pair of elements that are **out of their natural order**.
	- For instance: In $\{3,1,2\}$, pair $\{3,1\},\{3,2\}$ are inversions (逆序对).
#### **Orientation**
> of a simplex
- Is an *equivalence class* of **ordered tuples**, which must be either an **even** or **odd** permutation, except for **vertex** which has only one possible orientation.
#### **Consistent Orientation**
> for 2 *k-simplices*  $\sigma_1,\sigma_2$ share exactly $k$ vertices
- If their **restrictions** to these shared vertices (edge for 2 triangles, for instance) are **oppositely oriented** (k-1)-simplices. `Original text is k instead of k-1......`
#### **(Abstract) Oriented Simplicial Complex**
- Is an abstract simplicial complex where each simplex is **assigned an orientation**.
- It's often convenient to **assume** that k-simplices sharing common (k − 1)-faces have the **same orientation**.
	- For instance, that all triangles in a planar triangulation have **clockwise orientation**).
## 2.3 Simplicial Surfaces
#### **Manifold**
> An Abstract Simplicial Surface is *pure simplicial 2-complex* where:
- the **Link** of every vertex is a **single loop** of edges;
- the **Star** of every vertex is a **combinatorial disk** made of triangles, or a "disk-like" neighborhood.
- I suppose **n-complex** means complex comprised by simplices with the highest dimension of **n**.
#### **Nonmanifold**
> A simplicial surface with
- multiple triangles meeting at an edge
- multiple "cones" of vertices meeting at a vertex
#### **Simplicial N-Sphere**
- A (simplicial) **triangulation** of the n-dimensional sphere:
$$
\mathbb{S}^n:=\{x\in\mathbb{R}^{n+1}:|x|=1\}
$$
#### (Combinatorial or Abstract)**Simplicial N-Manifold**
-  A **pure simplicial n-complex** where the link of every vertex is a **simplicial (n − 1)-sphere**.
	- Simplicial **1**-Manifold: where every vertex's **link** is a simplicial 0-sphere, i.e. **a pair of points**.
	- Simplicial **2**-Manifold: where every vertex's **link** is a simplicial **1**-sphere, i.e. **a closed loop of edges**.
	- Simplicial **3**-Manifold: where every vertex's **link** is a simplicial **2**-sphere, i.e. **a triangulation of the sphere**.
## 2.4 Adjacency Matrices
#### **Adjacency Matrix**
$$
A_0=
\begin{bmatrix}
 1&-1& 0& 0\\
-1& 0& 1& 0\\
 0& 0&-1& 1\\
 0& 1& 0&-1\\
 0&-1& 1& 0\\
\end{bmatrix}
\quad
A_1=
\begin{bmatrix}
 1& 1& 0& 0&-1\\
 0& 0& 1& 1& 1\\
\end{bmatrix}
$$
- **Row number** mean number of **(k+1)-simplices**
	- $A_0$ have 5 rows also 5 **edges**
	- $A_1$ have 2 rows also 2 **triangles**
- **Column number** mean number of **k-simplices**
	- $A_0$ have 4 columns also 4 **vertices**
	- $A_1$ have 5 columns also 5 **edges**
- **1** in column **c** and row **r** of $\color{#8E7}{A_k}$ means **c**th (k+1)-simplex is **adjacent** to **r**th k-simplex, and have the **same orientation**
- **-1** in column **c** and row **r** of $\color{#8E7}{A_k}$ means **c**th (k+1)-simplex is **adjacent** to **r**th k-simplex, and have the **opposite orientation**
## 2.5 Halfedge Mesh
#### **Halfedge**
- For every **unoriented edge** $\{i,j\}$ between vertices $i$ and $j$, we have 2 **halfedges**: $ij\neq ji$.
- $|H|=2|E|$, H for **halfedges**, E for **edges**.
#### **Twin** function
- $\eta(ij)=ji$
#### **Next** function
- $\rho(ij)=jk, \forall ijk\in \mathcal K$
- orbits in **local** simplex
#### **Orbit**
- the **faces** are orbits of $\rho$
- the **edges** are orbits of $\eta$
- the **vertices** are orbits of $\rho \circ \eta$
	- twin, next, twin, next, ...
## 2.6 Written Exercises
#### **Euler Characteristic**
- $\mathcal{X}=V-E+F$
- Is a **Topological Invariant**
- for any **simplicial disk**: $V-E+F=1$
- for any **simplicial sphere**: $V-E+F=2$
#### **Genus**
- number of **handles**(torus, holes)
- among all surfaces that have no **boundary** and are:
	- **Connected**: meaning a single piece,
	- **Compact**: meaning closed and contained in a ball of finite size,
	- **Orientable**: meaning have two distinct sides,
	the **genus** is the **only** thing that distinguishes two surfaces.
#### **Euler-L'Huilier formula**
- $V-E+F=2-2genus$
- applies not only to **simplicial surfaces**, but surfaces made of general **n-gons** (squares, pentagons, etc.).
#### **Valence**
> of a **vertex** in a combinatorial surface
- is **the number of edges** that contain that vertex.
- A vertex of a *simplicial* surface is **regular** when its valence equals **6**.