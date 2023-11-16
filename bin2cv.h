#pragma once
#include <string>
#include <opencv2/opencv.hpp>

// AYA .bin  <-> cv::Mat のデータやり取り
// todo:  現状uinmt16_tだけなので 他データに拡張
// todo:  例外処理

namespace {
	template<typename T>
	T ReadStream(std::ifstream& ifs) {
		T data;
		ifs.read(reinterpret_cast<char*>(const_cast<T*>(&data)), sizeof(T));
		return data;
	};

	template<typename T>
	void WriteStream(std::ofstream& ofs, const T& data) {
		ofs.write(reinterpret_cast<char*>(const_cast<T*>(&data)), sizeof(T));
	}
}



namespace cv {

	//AYA binをcv::Mat CV_32Fで読み込む
	cv::Mat readBin(const std::string& filename)
	{
		cv::Mat out_matdata;
		std::ifstream ifs(filename, std::ios::binary);
		if (ifs.fail()) {
			puts("read .bin file failed !!");
			return out_matdata;
		}
		const int width = ReadStream<uint16_t>(ifs);
		const int height = ReadStream<uint16_t>(ifs);
		const size_t pixel_num = width * height;

		std::unique_ptr<uint16_t[]> pixel_buf(new uint16_t[pixel_num]);
		ifs.read((char*)pixel_buf.get(), sizeof(uint16_t) * pixel_num);
		ifs.close();

		out_matdata.create(height, width, CV_32F);
		auto* psrc = pixel_buf.get();
		for (int y = 0; y < height; ++y) {
			auto* src = out_matdata.ptr<float>(y);
			for (int x = 0; x < width; ++x) {
				src[x] = static_cast<float>(*psrc++);
			}
		}
		return out_matdata;
	}


	// AYA bin(uint_16t にキャストして書き出す)
	bool writeBin(const std::string& filename, const cv::Mat& img)
	{
		std::ofstream ofs(filename, std::ios::binary);
		if (ofs.fail()) {
			return false;
		}
		const int W = img.cols;
		const int H = img.rows;
		const int C = img.channels();

		const size_t pixel_num = W * H;
		std::unique_ptr<uint16_t[]> pixel_buf(new uint16_t[pixel_num]);
		auto* pdst = pixel_buf.get();

		for (int y = 0; y < H; ++y) {
			auto* psrc = img.ptr<float>(y);
			for (int x = 0; x < W; ++x) {
				*pdst++ = static_cast<uint16_t>(*psrc);
				psrc += C;
			}
		}
		WriteStream(ofs, static_cast<uint16_t>(W));
		WriteStream(ofs, static_cast<uint16_t>(H));
		ofs.write((char*)pixel_buf.get(), sizeof(uint16_t) * pixel_num);
		ofs.close();
		return true;
	}
}
