# So sánh hai quy trình xử lý ảnh: Với và Không có Zigzag, RLE, Huffman

## 1. Quy trình đầy đủ (giống JPEG chuẩn)

DCT → Quantization → Zigzag → RLE → Huffman
↓ ↑
InQuant ← Inverse Zigzag ← RLE Decode ← Huffman Decode
↓
Inverse DCT → reconstruct image

### Ý nghĩa:
- **DCT**: Biến đổi khối ảnh từ không gian sang miền tần số.
- **Quantization**: Giảm độ chính xác (loại bỏ thông tin ít quan trọng) → nén tốt hơn.
- **Zigzag**: Sắp xếp ma trận 8x8 theo đường chéo để gom hệ số 0 lại gần nhau.
- **RLE (Run-Length Encoding)**: Nén chuỗi số 0 liên tục → giảm size.
- **Huffman Coding**: Mã hóa các giá trị dựa trên tần suất xuất hiện → nén thêm.
- **InQuant**: Giải lượng tử để tái tạo gần đúng giá trị ban đầu.
- **Inverse DCT**: Chuyển ngược lại ảnh gốc từ miền tần số.

---

## 2. Quy trình rút gọn (Không nén sâu)

DCT → Quantization → InQuant → Inverse DCT → reconstruct image


### Ý nghĩa:
- Không có Zigzag, RLE, Huffman → chỉ thực hiện biến đổi và giảm chính xác.
- Không giảm kích thước file nhiều.
- Vẫn mất dữ liệu do Quantization.

---

## 3. So sánh hai quy trình

| Thành phần      | Có trong đầy đủ | Có trong rút gọn | Ý nghĩa & Tác động |
|-----------------|------------------|-------------------|--------------------|
| DCT             | ✅               | ✅                | Biến đổi sang tần số |
| Quantization    | ✅               | ✅                | Nén bằng cách giảm chi tiết |
| Zigzag          | ✅               | ❌                | Sắp xếp giúp dễ nén |
| RLE             | ✅               | ❌                | Nén chuỗi 0 |
| Huffman         | ✅               | ❌                | Nén bằng mã hóa tần suất |
| InQuant         | ✅               | ✅                | Giải lượng tử |
| Inverse DCT     | ✅               | ✅                | Phục hồi ảnh |

---

## 4. Tổng kết

| So sánh            | Quy trình đầy đủ         | Quy trình rút gọn       |
|--------------------|---------------------------|--------------------------|
| Độ nén             | Cao                       | Thấp hoặc không đáng kể |
| Kích thước file    | Nhỏ hơn nhiều             | Gần bằng ảnh gốc        |
| Tốc độ xử lý       | Chậm hơn (nhiều bước)     | Nhanh hơn               |
| Chất lượng ảnh     | Tùy thuộc vào lượng tử    | Cũng bị giảm             |
| Sử dụng thực tế    | Dùng trong JPEG chuẩn     | Thường chỉ để học thuật  |

---

## 5. Ghi chú
- Các bước **Zigzag → RLE → Huffman** không làm giảm thêm chất lượng ảnh mà chỉ giúp **giảm kích thước file** hiệu quả hơn.
- Nếu chỉ muốn học hoặc test DCT & Quantization, quy trình rút gọn là đủ.
- Nếu muốn nén ảnh như JPEG thật, cần thực hiện đầy đủ các bước.

---

