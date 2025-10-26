
# 📘 Bài 5: Hàm Hai Biến, Gradient, và Cực Trị

## 1️⃣ Hàm hai biến

Một hàm hai biến là ánh xạ:

$$
f : \mathbb{R}^2 \to \mathbb{R}, \quad f(x, y) = z
$$

Ví dụ:

$$
f(x, y) = x^2 + y^2
$$

Giá trị của hàm phụ thuộc đồng thời vào hai biến đầu vào.

---

## 2️⃣ Đạo hàm riêng

Đạo hàm riêng đo tốc độ thay đổi của hàm theo từng hướng $x$ hoặc $y$.

$$
\frac{\partial f}{\partial x} = \lim_{h \to 0} \frac{f(x + h, y) - f(x, y)}{h}
$$

$$
\frac{\partial f}{\partial y} = \lim_{h \to 0} \frac{f(x, y + h) - f(x, y)}{h}
$$

Tính gần đúng:

$$
f_x(x, y) ≈ \frac {f(x + h, y) - f(x - h, y)}{2h}
$$

$$
f_y(x, y) ≈ \frac {f(x, y + h) - f(x, y - h)}{2h}
$$

Ví dụ:

$$
f(x, y) = x^2 + 3xy + y^2
$$

Ta có:

$$
\frac{\partial f}{\partial x} = 2x + 3y, \quad \frac{\partial f}{\partial y} = 3x + 2y
$$

---

## 3️⃣ Gradient (∇f)

Gradient là vector chứa các đạo hàm riêng của hàm:

$$
\nabla f(x, y) = \left[
\frac{\partial f}{\partial x}, \frac{\partial f}{\partial y}
\right]
$$

Hướng của ∇f cho biết hướng tăng nhanh nhất của hàm $f$.  
Độ lớn của nó cho biết tốc độ tăng theo hướng đó.

---

## 4️⃣ Ma trận Hessian

Hessian mô tả độ cong của hàm:

$$
H(f) =
\begin{bmatrix}
\frac{\partial^2 f}{\partial x^2} & \frac{\partial^2 f}{\partial x \partial y} \\[6pt]
\frac{\partial^2 f}{\partial y \partial x} & \frac{\partial^2 f}{\partial y^2}
\end{bmatrix}
$$

Hessian giúp phân loại điểm cực trị (min, max, yên ngựa).

---

## 5️⃣ Phân loại điểm cực trị

Giả sử điểm tới hạn $(x_0, y_0)$ có $\nabla f(x_0, y_0) = 0$.  
Khi đó, xét **Hessian** tại điểm đó:

- Nếu $D = f_{xx} f_{yy} - f_{xy}^2 > 0$ và $f_{xx} > 0$ → **Cực tiểu**.  
- Nếu $D > 0$ và $f_{xx} < 0$ → **Cực đại**.  
- Nếu $D < 0$ → **Điểm yên ngựa**.  
- Nếu $D = 0$ → **Không kết luận được**.
---

## 6️⃣ Phương pháp Gradient Descent

Dùng để tìm điểm cực tiểu của hàm.

Cập nhật vị trí theo công thức:

$$
(x_{n+1}, y_{n+1}) = (x_n, y_n) - \alpha \nabla f(x_n, y_n)
$$

Trong đó:
- $\alpha > 0$: hệ số học (learning rate)  
- $\nabla f$: vector gradient tại điểm hiện tại

Quá trình lặp đến khi $|\nabla f| < \varepsilon$.

---

## 7️⃣ Minh họa đồ thị và vector gradient

- Đồ thị 3D: bề mặt của $f(x,y)$
- Vector gradient (∇f): mũi tên hướng theo chiều tăng nhanh nhất.
- Trong mô phỏng, vẽ các mũi tên ngược chiều gradient để thấy hướng **giảm dần**.

---

## 8️⃣ Tổng kết

| Thành phần | Ký hiệu | Ý nghĩa |
|-------------|----------|---------|
| Hàm hai biến | $f(x, y)$ | Đầu vào 2 biến, đầu ra 1 giá trị |
| Đạo hàm riêng | $\frac{\partial f}{\partial x}, \frac{\partial f}{\partial y}$ | Độ dốc theo từng trục |
| Gradient | $\nabla f = [f_x, f_y]$ | Hướng tăng nhanh nhất |
| Hessian | $H(f)$ | Ma trận cong bậc hai |
| Gradient Descent | $x_{n+1} = x_n - \alpha \nabla f$ | Tìm cực tiểu |