#include <iostream>
#include <gdal/gdal_priv.h>

int main(int argc, char *argv[])
{
	GDALAllRegister();

	const char *filename = argv[1]; //"path_to_noaa_dem.tif";
	GDALDataset *dataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);
	if (dataset == nullptr) {
		std::cerr << "Failed to open file\n";
		return -1;
	}

	const char *projRef = dataset->GetProjectionRef();
	if (projRef != nullptr && strlen(projRef) > 0) {
		std::cout << "Projection is:\n"
			<< projRef << "\n";
	} else {
		std::cerr << "No projection information found\n";
	}

	double adfGeoTransform[6];
	if (dataset->GetGeoTransform(adfGeoTransform) == CE_None) {
		std::cout << "Top left X: " << adfGeoTransform[0] << "\n";
		std::cout << "Pixel width: " << adfGeoTransform[1] << " PCS units\n";
		std::cout << "Rotation (0 if north-up): " << adfGeoTransform[2] << "\n";
		std::cout << "Top left Y: " << adfGeoTransform[3] << "\n";
		std::cout << "Rotation (0 if north-up): " << adfGeoTransform[4] << "\n";
		std::cout << "Pixel height: " << adfGeoTransform[5] << " PCS units\n";
	}

	GDALClose(dataset);
	return 0;
}
