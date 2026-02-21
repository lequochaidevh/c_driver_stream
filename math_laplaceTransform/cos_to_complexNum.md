# Từ Chuỗi Taylor đến Cos(t) và Biến Đổi Laplace

## 1. Chuỗi Taylor (Maclaurin)

Với hàm số khả vi vô hạn tại 0, chuỗi Taylor (Maclaurin) được phát biểu:

\[
f(x)=\sum_{n=0}^{\infty}\frac{f^{(n)}(0)}{n!}x^n
\]

---

## 2. Chuỗi Taylor của hàm mũ

Hàm mũ được định nghĩa bởi chuỗi:

\[
e^x=\sum_{n=0}^{\infty}\frac{x^n}{n!}
\]

Chuỗi này hội tụ với mọi \(x \in \mathbb{R}\) và cả \(x \in \mathbb{C}\).

---

## 3. Thay biến phức \(x = it\)

Xét:

\[
e^{it}=\sum_{n=0}^{\infty}\frac{(it)^n}{n!}
\]

Tách các số hạng **chẵn** và **lẻ**:

### 3.1. Số hạng chẵn

\[
\sum_{n=0}^{\infty}\frac{(it)^{2n}}{(2n)!}
=\sum_{n=0}^{\infty}(-1)^n\frac{t^{2n}}{(2n)!}
\]

### 3.2. Số hạng lẻ

\[
\sum_{n=0}^{\infty}\frac{(it)^{2n+1}}{(2n+1)!}
=i\sum_{n=0}^{\infty}(-1)^n\frac{t^{2n+1}}{(2n+1)!}
\]

---

## 4. Định nghĩa sin và cos bằng chuỗi Taylor

Từ các chuỗi trên, ta **định nghĩa**:

### Cosine:
\[
\boxed{
\cos t=\sum_{n=0}^{\infty}(-1)^n\frac{t^{2n}}{(2n)!}
}
\]

### Sine:
\[
\boxed{
\sin t=\sum_{n=0}^{\infty}(-1)^n\frac{t^{2n+1}}{(2n+1)!}
}
\]

---

## 5. Công thức Euler

Ghép các kết quả trên:

\[
\boxed{
e^{it}=\cos t+i\sin t
}
\]

Từ đó suy ra:

\[
\boxed{
\cos t=\frac{e^{it}+e^{-it}}{2}
}
\]

---

## 6. Định nghĩa biến đổi Laplace

Với hàm \(f(t)\), biến đổi Laplace được định nghĩa:

\[
\mathcal{L}\{f(t)\}
=\int_{0}^{\infty}e^{-st}f(t)\,dt,
\quad \Re(s)>0
\]

---

## 7. Biến đổi Laplace của \(e^{at}\)

Với \(a \in \mathbb{C}\):

\[
\mathcal{L}\{e^{at}\}
=\int_{0}^{\infty}e^{-(s-a)t}\,dt
=\frac{1}{s-a},
\quad \Re(s)>\Re(a)
\]

---

## 8. Biến đổi Laplace của cos(t)

Từ biểu diễn mũ:

\[
\cos t=\frac{e^{it}+e^{-it}}{2}
\]

Áp dụng Laplace:

\[
\mathcal{L}\{\cos t\}
=\frac{1}{2}\left(
\frac{1}{s-i}+\frac{1}{s+i}
\right)
\]

Quy đồng:

\[
\mathcal{L}\{\cos t\}
=\frac{s}{s^2+1},
\quad \Re(s)>0
\]

---

## 9. Kết luận

- Chuỗi Taylor của \(e^x\) sinh ra tự nhiên sin và cos khi thay biến phức
- Cosine được định nghĩa bằng chuỗi các số hạng chẵn
- Biểu diễn mũ cho phép tính biến đổi Laplace một cách trực tiếp
- Kết quả cuối cùng:

\[
\boxed{
\mathcal{L}\{\cos t\}=\frac{s}{s^2+1}
}
\]

---

## 10. Ý nghĩa

Cách tiếp cận này là nền tảng trong:
- Giải tích phức
- Phương trình vi phân
- Mạch điện & điều khiển tự động
- Xử lý tín hiệu
