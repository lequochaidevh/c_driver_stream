# JPEG Compression Pipeline Analysis

## 1. Tổng quan

Khi nén ảnh, có hai quy trình phổ biến:

### A. Quy trình đầy đủ (JPEG chuẩn)
```
DCT → Quantization → Zigzag → RLE → Huffman → Inverse Huffman → Inverse RLE → De-Zigzag → Inverse Quantization → Inverse DCT
```

### B. Quy trình rút gọn (bỏ nén entropy)
```
DCT → Quantization → Inverse Quantization → Inverse DCT
```

Hai pipeline này khác nhau chủ yếu ở **mức độ nén và mất mát dữ liệu**.

---

## 2. Giải thích từng bước

| Bước | Mô tả | Ghi chú |
|------|--------|---------|
| **DCT (Discrete Cosine Transform)** | Biến đổi ảnh từ miền không gian → miền tần số. | Các hệ số tần số cao thường nhỏ, dễ bỏ bớt khi nén. |
| **Quantization** | Chia các hệ số DCT cho ma trận lượng tử, rồi làm tròn. | Bước gây **mất mát dữ liệu** (lossy). |
| **Zigzag scan** | Sắp xếp hệ số từ thấp → cao tần theo đường zigzag. | Giúp gom nhiều số 0 liên tiếp để nén tốt hơn. |
| **RLE (Run-Length Encoding)** | Nén dãy 0 liên tiếp. | Giảm kích thước dữ liệu. |
| **Huffman coding** | Mã hóa entropy tối ưu. | Giảm kích thước thêm, **không mất mát**. |
| **Inverse Quantization** | Nhân lại với ma trận lượng tử. | Không khôi phục hoàn toàn giá trị ban đầu do đã làm tròn. |
| **Inverse DCT** | Biến ngược miền tần số về miền không gian. | Sinh lại ảnh nén. |

---

## 3. So sánh hai pipeline

| Tiêu chí | DCT → Quant → Zigzag → RLE → Huffman | DCT → Quant → InQuant |
|-----------|--------------------------------------|------------------------|
| **Mục tiêu** | Nén ảnh (JPEG chuẩn) | Kiểm tra tác động của lượng tử hóa |
| **Nén dữ liệu** | Có, hiệu quả cao | Không đáng kể |
| **Mất mát dữ liệu** | Có (do Quantization) | Có (do Quantization), nhưng không do entropy coding |
| **Khôi phục ảnh** | Cần giải mã đầy đủ (Huffman, RLE, v.v.) | Nhanh, dễ thực hiện |
| **Kích thước ảnh nén** | Nhỏ hơn nhiều (vài KB) | Gần như bằng ảnh gốc (MB) |
| **Độ méo ảnh (PSNR)** | Giảm do lượng tử hóa | Tương tự, không khác biệt nhiều |
| **Ứng dụng thực tế** | JPEG, MJPEG | Thử nghiệm thuật toán, nghiên cứu |

---

## 4. Ví dụ minh họa

Giả sử một block 8×8 có DCT như sau (giá trị giả định):

|   |   |   |   |   |   |   |   |
|---|---|---|---|---|---|---|---|
| 100 | 20 | 10 | 5 | 2 | 1 | 0 | 0 |
| 10 | 5 | 2 | 1 | 0 | 0 | 0 | 0 |
| ... | ... | ... | ... | ... | ... | ... | ... |

Sau **Quantization** (chia cho ma trận lượng tử, rồi làm tròn):

```
[[8, 2, 1, 0, 0, 0, 0, 0],
 [1, 0, 0, 0, 0, 0, 0, 0],
 ... ]
```

Sau **Zigzag + RLE + Huffman**, block này có thể chỉ còn vài byte dữ liệu.

Nếu chỉ **DCT → Quant → InQuant → IDCT**, ta vẫn được ảnh gần giống ảnh sau giải mã JPEG, nhưng **kích thước dữ liệu chưa được nén**, vì bỏ qua phần entropy coding.

---

## 5. Kết luận

| Pipeline | Ưu điểm | Nhược điểm |
|-----------|----------|-------------|
| **Đầy đủ (JPEG)** | Kích thước nhỏ, chuẩn nén thực tế | Giải mã phức tạp hơn |
| **Rút gọn (Không entropy coding)** | Đơn giản, dễ debug thuật toán DCT/Quant | Không nén được kích thước file |

✅ **Tóm lại:**
- DCT + Quantization quyết định **chất lượng ảnh sau nén** (lossy part).  
- Zigzag + RLE + Huffman chỉ ảnh hưởng đến **kích thước file**, **không làm mất thêm thông tin**.  
- Khi **không dùng Zigzag/RLE/Huffman**, ảnh sau InQuant + Inverse DCT sẽ có **chất lượng tương đương** ảnh JPEG giải mã, chỉ khác ở **kích thước lưu trữ**.

---

## 6. Tài liệu tham khảo
- ITU-T Recommendation T.81 (JPEG Standard)
- Gonzalez & Woods, *Digital Image Processing*
- JPEG Explained – W3C, ISO/IEC JTC1/SC29/WG1
