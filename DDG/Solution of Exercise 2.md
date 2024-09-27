> Just try it. Not guaranteed to be correct.
## 1. Euler's Polyhedral Formula - Simplicial
> Induction
- If a *Euler characteristic* suites for a simplicial disk with **F** faces, then it must suites for disk with **F-1** faces which reduced from the last one, since it's not a *global* change. Eventually, it must suites for disk just have one triangle, which have **3** vertices, **3** edges, and **1** faces, then:

$$
V-E+F=3-3+1=1.
$$
- A sphere is equivalent to a **tetrahedron** which have **4** vertices, **6** edges, and **4** faces, then:

$$
V-E+F=4-6+4=2.
$$
## 2. Platonic Solids
$$
\begin{align}
let \quad n:&= sides\,of\,polygon\\
let \quad f:&= numbers\,of\,faces\\
let \quad v:&= valence\,of\,vertex\\
then \quad F&=f\\
E&=\frac{fn}{2}\\
V&=\frac{fn}{v}\\
while \,V-E+F&=2 \\
then \, \frac{fn}{v}-\frac{fn}{2}+f&=2\\
f&=\frac{4v}{2n-nv+2v}
\end{align}
$$
- For polyhedra with **triangles**, $n=3$
	- when $v=3, f=4$;
	- when $v=4, f=8$;
	- when $v=5, f=20$;
	- v couldn't bigger than 5, cause $60\degree\times6\ge360\degree$.
- For polyhedra with **squares**, $n=4$
	- when $v=3, f=6$;
	- v couldn't bigger than 3, cause $90\degree\times4\ge360\degree$.
- For polyhedra with **pentagons**, $n=5$
	- when $v=3, f=12$;
	- when $v=4, f=-8$, no a possitive integer;
	- v couldn't bigger than 5, cause $72\degree\times5\ge360\degree$.
- For polyhedra with **hexagon**, $n=6$
	- v couldn't bigger than 2, cause $120\degree\times3\ge360\degree$.
- So for polyhedra with face which n **greater than 6**.
## 3. Regular Valence
$$
\begin{align}
V-E+F&=2-2g\\
\frac{fn}{v}-\frac{fn}{2}+f&=2-2g\\
Let \, n=3,v&=6\\
then \, g&=1
\end{align}
$$
## 4. Minimum Irregular Valence
TODO
## 5. Mean Valence (Triangle Mesh)
$$
\begin{align}
V-F+F&=2-2g\\
\frac{fn}{v}-\frac{fn}{2}+f&=2-2g\\
when\,n=3,v&=6\\
\frac{3}{6}-\frac{3}{2}+1&=\frac{2-2g}{f}\\
\frac36:\frac32:1&=1:3:2
\end{align}
$$
## 6. Mean Valence (Quad Mesh)
$$
\frac{Qn}{v}:\frac{Qn}{2}:Q=\frac46:\frac42:1=2:6:3
$$
## 7. Mean Valence (Tetrahedral)
- Pretend that the link Lk(i) of every vertex $i\in V$ is a combinatorial **icosahedron**.
- Icosahedron have $\frac{20\times 3}{2}=30$ **edges**.
- Icosahedron have 20 **tetrahedrals**.
- Since there is no boundary, the number of **edges** in St(i) of every vertex $i\in V$ is **12**.
- Since there is no boundary, the number of **triangles** in St(i) of every vertex $i\in V$ is **30**.

$$
\begin{align}
V-E+F-T&=c\\
V:E:F:T=V:\frac{12V}{2}:\frac{30V}{3}:\frac{20V}{4}&=1:6:10:5
\end{align}
$$
## 8. Star, Closure, and Link
> [Simplicial Complex Operators (geometrycollective.github.io)](https://geometrycollective.github.io/geometry-processing-js/projects/simplicial-complex-operators/index.html)
- Star
	- Vertices: {e}, {k}, {o}
	- Edges: {ea}, {eb}, {ef}, {ek}, {ej}, {ed}, {jk}, {kp}, {fo}
	- Triangles: {eab}, {ebf}, {efk}, {ekj}, {ejd}, {eda}, {ofk}, {olf}, {okp}, {jkp}, {njp}
- Closure
	- complex: {pnjk}, {e}, {fo}
- Link
	- {da}, {ab}, {ql}
## 9. Boundary and Interior
- Boundary
	- {a}, {ab}, {b}, {bf}, {f}, {fl}, {l}, {lq}, {q}, {qp}, {p}, {pj}, {j}, {je}, {e}, {ea}
- Interior
	- {k}, {o}
	- {eb}, {ef}, {ek}, {kf}, {jk}, {kp}, {fo}, {ko}, {op}, {oq}, {ol}, {lq}, {qm}
	- {aeb}, {ebf}, {efk}, {ekj}, {kfo}, {jkp}, {kop}, {fol}, {opq}, {olq}, {lqm}, {qsm}
## 10. Surface as Permutation
| h         | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   |
| --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| $\eta(h)$ | 4   | 2   | 1   | 5   | 0   | 3   | 7   | 6   | 9   | 8   |
| $\rho(h)$ | 1   | 2   | 0   | 4   | 5   | 6   | 3   | 9   | 7   | 8   |
## 11. Permutation as Surface
- It's a **Pyramid**.
## 12. Surface as Matrices
$$
A_0=
\begin{bmatrix}
1&1&0&0&0\\
1&0&1&0&0\\
1&0&0&1&0\\
1&0&0&0&1\\
0&1&0&0&1\\
0&1&1&0&0\\
0&1&0&1&0\\
0&0&0&1&1\\
\end{bmatrix}
\quad
A_1=
\begin{bmatrix}
1&0&0&1&1&0&0&0\\
1&1&0&0&0&1&0&0\\
0&1&1&0&0&0&1&0\\
0&0&1&1&0&0&0&1\\
\end{bmatrix}
$$
## 13. Classification of Simplicial 1-Manifolds
- Can not have multiple **edges** meeting at an **vertex**.
## 14. Boundary Loops
- Every simplex that is **proper face** is contained in the boundary. There would not be any **holes**.
## 15. Boundary Has No Boundary
- There is no **proper face** in the **closure** of the boundary.