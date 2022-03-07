#include <cstdlib>
#include <string>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkNiftiImageIO.h>

itk::ImageIOBase::Pointer getImageIO(std::string input){
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(input.c_str(), itk::ImageIOFactory::ReadMode);

    imageIO->SetFileName(input);
    imageIO->ReadImageInformation();

    return imageIO;
}

itk::ImageIOBase::IOComponentType component_type(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetComponentType();
}

itk::ImageIOBase::IOPixelType pixel_type(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetPixelType();
}

size_t num_dimensions(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetNumberOfDimensions();
}

int main(int argc, char const *argv[]){
    if(argc < 2){
        std::cerr << "Required File Name"<<std::endl;
        return EXIT_FAILURE;
    }
    itk::ImageIOBase::Pointer imageIO = getImageIO(argv[1]);
    std::cout<<"Num of Dimensions : "<<num_dimensions(imageIO)<<std::endl;
    std::cout<<"Component Type :"<<imageIO->GetComponentTypeAsString(component_type(imageIO))<<std::endl;
    std::cout<<"Pixel Type :"<<imageIO->GetPixelTypeAsString(pixel_type(imageIO))<<std::endl;

    if(num_dimensions(imageIO) == 4 && component_type(imageIO) == imageIO->GetComponentTypeFromString("short"))
    {
        typedef itk::Image<short, 4> DWI;
        typedef itk::ImageFileReader<DWI> DWIReader;

        DWIReader::Pointer reader = DWIReader::New();
        reader->SetFileName(imageIO -> GetFileName());
        reader->Update();

        DWI::RegionType region;
        DWI::IndexType start;
        DWI::SizeType size;

        for(int i =0; i < num_dimensions(imageIO); i++){
            start[i] = 0;
            size[i] = imageIO->GetDimensions(i);
        }

        region.SetSize(size);
        region.SetIndex(start);
        DWI::Pointer dwi = reader->GetOutput();
        DWI::Pointer out_dwi = DWI::New();

        out_dwi->SetRegions(region);
        out_dwi->Allocate();

        itk::ImageRegionIterator<DWI> it(dwi, region);

        while(!it.IsAtEnd()){
            out_dwi->SetPixel(it.GetIndex(), it.Value());
            ++it;
        }
        itk::NiftiImageIO::Pointer nifti_io = itk::NiftiImageIO::New();
        nifti_io->SetPixelType(pixel_type(imageIO));

        itk::ImageFileWriter<DWI>::Pointer writer = itk::ImageFileWriter<DWI>::New();
        writer->SetFileName("test.nii.gz");
        writer->SetInput(out_dwi);
        writer->SetImageIO(nifti_io);
        writer->Update();
    }
    return EXIT_SUCCESS;
}