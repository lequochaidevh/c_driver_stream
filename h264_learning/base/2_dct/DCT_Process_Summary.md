# 🧠 TÓM TẮT QUY TRÌNH XỬ LÝ ẢNH BẰNG DCT

## 1️⃣ Đọc ảnh PGM gốc
- Hàm `read_pgm()` đọc ảnh xám `.pgm` (kiểu nhị phân `P5`).
- Kết quả: mảng pixel `Y[WIDTH * HEIGHT]` giá trị 0–255.  
👉 **Mục đích:** lấy dữ liệu gốc để xử lý.

---

## 2️⃣ Chia ảnh thành các khối 8×8
- Ảnh được chia đều thành các block `(bxCount × byCount)`.
- Mỗi block chứa 64 giá trị pixel.  
👉 **Mục đích:** DCT hoạt động trên khối nhỏ, giúp nén hiệu quả.

---

## 3️⃣ Biến đổi Cosine Rời rạc (DCT)
- Dùng công thức 2D DCT để chuyển từ **miền không gian → miền tần số**.  
- Mỗi block cho ra ma trận 8×8 hệ số DCT.
- Hệ số `[0][0]` là **DC coefficient** → biểu diễn độ sáng trung bình của block.  
👉 **Mục đích:** tách phần năng lượng thấp (DC, quan trọng) và cao (AC, ít quan trọng).

---

## 4️⃣ Lượng tử hóa (Quantization)
- Chia từng hệ số DCT cho ma trận lượng tử hóa (ví dụ như JPEG).  
- Làm tròn kết quả → giảm dữ liệu cần lưu.  
👉 **Mục đích:** nén, loại bỏ chi tiết tần số cao mà mắt ít nhận thấy.

---

## 5️⃣ Giải lượng tử hóa (Dequantization)
- Nhân lại với ma trận lượng tử hóa để khôi phục gần đúng giá trị DCT ban đầu.  
👉 **Mục đích:** chuẩn bị cho bước tái tạo ảnh.

---

## 6️⃣ Biến đổi Cosine ngược (IDCT)
- Áp dụng công thức IDCT để chuyển lại về miền không gian (pixel).  
👉 **Mục đích:** tái tạo từng khối ảnh sau khi nén.

---

## 7️⃣ Ghép lại khung hình
```c
allBlocks[by * bxCount + bx][i][j] = idct[i][j];
```
- Ghép tất cả block 8×8 lại thành ảnh đầy đủ (`reconstructed.pgm`).  
👉 **Mục đích:** tạo lại ảnh gần giống ảnh gốc sau khi qua DCT-IDCT.

---

## 8️⃣ Tạo bản đồ nhiệt DC (dc_heatmap.pgm)
- Mỗi block lấy giá trị DC (độ sáng trung bình).
- Ghép chúng lại thành ảnh nhỏ hơn (theo kích thước block).  
👉 **Mục đích:** trực quan hóa độ sáng của từng vùng ảnh.

---

## 9️⃣ Kết quả đầu ra

| File | Ý nghĩa |
|------|----------|
| `dct_blocks.csv` | Ghi các hệ số DCT của từng block (trước/ sau lượng tử hóa). |
| `reconstructed.pgm` | Ảnh tái tạo sau IDCT. |
| `dc_heatmap.pgm` | Ảnh bản đồ nhiệt DC – vùng sáng/tối theo block. |

---

## 🔟 Lưu ý lỗi từng gặp
- Thiếu `#include <string.h>` → lỗi `strcmp`.
- Ghép sai vị trí block → hình bị lệch (sửa bằng đúng thứ tự `by * bxCount + bx`).
- Dữ liệu DC quá nhỏ hoặc đảo → kiểm tra bước DCT / lượng tử.

---
