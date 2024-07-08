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
	if (projRef != nullptr) {
		std::cout << "Projection is:\n"
			<< projRef << "\n";
	} else {
		std::cerr << "No projection information found\n";
	}

	GDALClose(dataset);
	return 0;
}
