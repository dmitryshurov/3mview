
################################################################################
# External dependency: ITK library
################################################################################

IF(WIN32)
    # include paths
    IF(NOT EXISTS ${MOD_DIR}/ext/InsightToolkit/include/itkConfigure.h)
        MESSAGE(FATAL_ERROR "ITK headers not found (modules/itk/ext/InsightToolkit/include/itkConfigure.h). "
            "Copy ITK 4.0/4.1 library to modules/itk/ext/ (see http://voreen.uni-muenster.de)")
    ENDIF()
    SET(MOD_INCLUDE_DIRECTORIES 
        ${MOD_DIR}/ext/InsightToolkit/include
        ${MOD_DIR}/ext/InsightToolkit/include/Algorithms
        ${MOD_DIR}/ext/InsightToolkit/include/BasicFilters
        ${MOD_DIR}/ext/InsightToolkit/include/Common
        ${MOD_DIR}/ext/InsightToolkit/include/gdcm
        ${MOD_DIR}/ext/InsightToolkit/include/IO
        ${MOD_DIR}/ext/InsightToolkit/include/Numerics
        ${MOD_DIR}/ext/InsightToolkit/include/Numerics/Statistics
        ${MOD_DIR}/ext/InsightToolkit/include/Review
        ${MOD_DIR}/ext/InsightToolkit/include/SpatialObject
        ${MOD_DIR}/ext/InsightToolkit/include/Utilities
        ${MOD_DIR}/ext/InsightToolkit/include/Utilities/vxl/vcl
        ${MOD_DIR}/ext/InsightToolkit/include/Utilities/vxl/core
    )

    # libraries
    IF(VRN_WIN32)
        SET(ITK_LIB_DIR ${MOD_DIR}/ext/InsightToolkit/lib/win32)
    ELSEIF(VRN_WIN64)
        SET(ITK_LIB_DIR ${MOD_DIR}/ext/InsightToolkit/lib/win64)
    ELSE()
        MESSAGE(FATAL_ERROR "Neither VRN_WIN32 nor VRN_WIN64 defined!")
    ENDIF()

    IF(EXISTS ${ITK_LIB_DIR}/Debug/ITKCommon-4.0.lib)
        MESSAGE(STATUS "  - Found ITK 4.0 library")
        SET(ITK_VERSION 4.0)
    ELSEIF(EXISTS ${ITK_LIB_DIR}/Debug/ITKCommon-4.1.lib)
        MESSAGE(STATUS "  - Found ITK 4.1 library")
        SET(ITK_VERSION 4.1)
    ELSE()
        MESSAGE(FATAL_ERROR "ITK library not found. Copy ITK 4.0/4.1 library to modules/itk/ext/ " 
            "(see http://voreen.uni-muenster.de)")
    ENDIF()

    SET(MOD_DEBUG_LIBRARIES 
        ${ITK_LIB_DIR}/Debug/ITKBiasCorrection-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKBioCell-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKCommon-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKDeprecated-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKDICOMParser-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKEXPAT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKFEM-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Debug/ITKFFT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmCommon-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmDICT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmDSED-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmIOD-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmjpeg8-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmjpeg12-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmjpeg16-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkgdcmMSFF-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKgiftiio-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkhdf5_cpp-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkhdf5-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Debug/ITKHighDimensionalMetrics-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Debug/ITKHighDimensionalOptimizers-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Debug/ITKIOBase-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOBioRad-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOBMP-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOCSV-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOGDCM-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOGE-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOGIPL-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOHDF5-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOIPL-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOJPEG-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOLSM-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOMeta-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIONIFTI-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIONRRD-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOPNG-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOSiemens-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOSpatialObjects-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOStimulate-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOTIFF-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOVTK-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKIOXML-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkjpeg-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKKLMRegionGrowing-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKLabelMap-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKMetaIO-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkNetlibSlatec-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKniftiio-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKNrrdIO-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkopenjpeg-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKOptimizers-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKPath-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkpng-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKPolynomials-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKQuadEdgeMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKReview-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKSpatialObjects-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKStatistics-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itksys-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itktiff-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkv3p_lsqr-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkv3p_netlib-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkvcl-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkvnl_algo-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkvnl-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKVNLInstantiation-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKVTK-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKWatersheds-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/itkzlib-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Debug/ITKznz-${ITK_VERSION}.lib
    )
        
    SET(MOD_RELEASE_LIBRARIES 
        ${ITK_LIB_DIR}/Release/ITKBiasCorrection-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKBioCell-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKCommon-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKDeprecated-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKDICOMParser-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKEXPAT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKFEM-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Release/ITKFFT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmCommon-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmDICT-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmDSED-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmIOD-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmjpeg8-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmjpeg12-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmjpeg16-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkgdcmMSFF-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKgiftiio-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkhdf5_cpp-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkhdf5-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Release/ITKHighDimensionalMetrics-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Release/ITKHighDimensionalOptimizers-${ITK_VERSION}.lib
        #${ITK_LIB_DIR}/Release/ITKIOBase-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOBioRad-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOBMP-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOCSV-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOGDCM-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOGE-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOGIPL-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOHDF5-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOIPL-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOJPEG-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOLSM-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOMeta-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIONIFTI-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIONRRD-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOPNG-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOSiemens-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOSpatialObjects-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOStimulate-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOTIFF-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOVTK-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKIOXML-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkjpeg-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKKLMRegionGrowing-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKLabelMap-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKMetaIO-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkNetlibSlatec-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKniftiio-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKNrrdIO-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkopenjpeg-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKOptimizers-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKPath-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkpng-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKPolynomials-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKQuadEdgeMesh-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKReview-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKSpatialObjects-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKStatistics-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itksys-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itktiff-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkv3p_lsqr-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkv3p_netlib-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkvcl-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkvnl_algo-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkvnl-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKVNLInstantiation-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKVTK-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKWatersheds-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/itkzlib-${ITK_VERSION}.lib
        ${ITK_LIB_DIR}/Release/ITKznz-${ITK_VERSION}.lib
    )
    
    IF(EXISTS ${ITK_LIB_DIR}/Debug/ITKIOImageBase-${ITK_VERSION}.lib)
        LIST(APPEND MOD_DEBUG_LIBRARIES
            ${ITK_LIB_DIR}/Debug/ITKIOImageBase-${ITK_VERSION}.lib
            ${ITK_LIB_DIR}/Debug/ITKIOTransformBase-${ITK_VERSION}.lib
        )
        LIST(APPEND MOD_RELEASE_LIBRARIES
            ${ITK_LIB_DIR}/Release/ITKIOImageBase-${ITK_VERSION}.lib
            ${ITK_LIB_DIR}/Release/ITKIOTransformBase-${ITK_VERSION}.lib
        )
    ENDIF()

    SET(MOD_DEBUG_DLLS
        ${ITK_LIB_DIR}/Debug/ITKCommon-${ITK_VERSION}.dll
    )
    SET(MOD_RELEASE_DLLS
        ${ITK_LIB_DIR}/Release/ITKCommon-${ITK_VERSION}.dll
    )

    
ELSEIF(UNIX)
    # TODO: Use FindITK
    
    # include paths
    SET(MOD_INCLUDE_DIRECTORIES
        /usr/local/include/ITK-4.0/
        /usr/local/include/InsightToolkit
        /usr/local/include/InsightToolkit/Algorithms
        /usr/local/include/InsightToolkit/BasicFilters
        /usr/local/include/InsightToolkit/Common
        /usr/local/include/InsightToolkit/IO
        /usr/local/include/InsightToolkit/Numerics
        /usr/local/include/InsightToolkit/Numerics/Statistics
        /usr/local/include/InsightToolkit/Review
        /usr/local/include/InsightToolkit/SpatialObject
        /usr/local/include/InsightToolkit/Utilities
        /usr/local/include/InsightToolkit/Utilities/vxl/core
        /usr/local/include/InsightToolkit/Utilities/vxl/vcl
    )

    # link against ITK V4.0
    SET(MOD_LIBRARIES
        -L/usr/local/lib
        -lITKBiasCorrection-4.0
        -lITKBioCell-4.0
        -lITKCommon-4.0
        -lITKDeprecated-4.0
        -lITKDICOMParser-4.0
        -lITKEXPAT-4.0
        -lITKFEM-4.0
        -litkgdcmCommon-4.0
        -litkgdcmDICT-4.0
        -litkgdcmDSED-4.0
        -litkgdcmIOD-4.0
        -litkgdcmjpeg12-4.0
        -litkgdcmjpeg16-4.0
        -litkgdcmjpeg8-4.0
        -litkgdcmMSFF-4.0
        -litkgdcmuuid-4.0
        -lITKgiftiio-4.0
        -litkhdf5-4.0
        -litkhdf5_cpp-4.0

        -lITKIOBioRad-4.0
        -lITKIOBMP-4.0
        -lITKIOCSV-4.0
        -lITKIOGDCM-4.0
        -lITKIOGE-4.0
        -lITKIOGIPL-4.0
        -lITKIOHDF5-4.0
        -lITKIOIPL-4.0
        -lITKIOJPEG-4.0
        -lITKIOLSM-4.0
        -lITKIOMesh-4.0
        -lITKIONIFTI-4.0
        -lITKIONRRD-4.0
        -lITKIOPNG-4.0
        -lITKIOSiemens-4.0
        -lITKIOSpatialObjects-4.0
        -lITKIOStimulate-4.0
        -lITKIOTIFF-4.0
        -lITKIOVTK-4.0
        -lITKIOXML-4.0
        -lITKIOMeta-4.0
        -lITKIOTransformHDF5-4.0
        -lITKIOTransformInsightLegacy-4.0
        -lITKIOTransformMatlab-4.0
        -lITKIOTransformBase-4.0
        -lITKIOImageBase-4.0

        -litkjpeg-4.0
        -lITKKLMRegionGrowing-4.0
        -lITKLabelMap-4.0
        -lITKMesh-4.0
        -lITKMetaIO-4.0
        -litkNetlibSlatec-4.0
        -lITKniftiio-4.0
        -lITKNrrdIO-4.0
        -litkopenjpeg-4.0
        -lITKOptimizers-4.0
        -lITKOptimizersv4-4.0
        -lITKPath-4.0
        -litkpng-4.0
        -lITKPolynomials-4.0
        -lITKQuadEdgeMesh-4.0
        -lITKReview-4.0
        -lITKSpatialObjects-4.0
        -lITKStatistics-4.0
        -litksys-4.0
        -litktiff-4.0
        -litkv3p_lsqr-4.0
        -litkv3p_netlib-4.0
        -litkvcl-4.0
        -lITKVideoCore-4.0
        -lITKVideoIO-4.0
        -litkvnl-4.0
        -litkvnl_algo-4.0
        -lITKVNLInstantiation-4.0
        -lITKVTK-4.0
        -lITKWatersheds-4.0
        -litkzlib-4.0
        -lITKznz-4.0
    )
    
ENDIF()


################################################################################
# Core module resources 
################################################################################
SET(MOD_CORE_MODULECLASS ITKModule)

SET(MOD_CORE_SOURCES 
    ${MOD_DIR}/utils/itkwrapper.cpp
    #${MOD_DIR}/io/itkvolumereader.cpp
    ${MOD_DIR}/processors/anisotropicdiffusion.cpp
    ${MOD_DIR}/processors/doublethreshold.cpp
    ${MOD_DIR}/processors/gradientvectorflow.cpp
    ${MOD_DIR}/processors/itkprocessor.cpp
    ${MOD_DIR}/processors/volumefilter_itk.cpp
    ${MOD_DIR}/processors/valuedregionalmaximaimagefilter.cpp
    ${MOD_DIR}/processors/vesselness.cpp
    ${MOD_DIR}/processors/mutualinformationregistration.cpp
    ${MOD_DIR}/processors/fastmarchingimagefilter.cpp
    ${MOD_DIR}/processors/curveslevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/geodesicactivecontourlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/geodesicactivecontourshapepriorlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/narrowbandcurveslevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/narrowbandthresholdsegmentationlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/shapedetectionlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/watershedimagefilter.cpp
    ${MOD_DIR}/processors/thresholdlabelerimagefilter.cpp
    ${MOD_DIR}/processors/labelmapoverlayimagefilter.cpp
    ${MOD_DIR}/processors/labelmapcontouroverlayimagefilter.cpp
    ${MOD_DIR}/processors/bayesianclassifierimagefilter.cpp
)

SET(MOD_CORE_HEADERS 
    ${MOD_DIR}/utils/itkwrapper.h
    #${MOD_DIR}/io/itkvolumereader.h
    ${MOD_DIR}/processors/anisotropicdiffusion.h
    ${MOD_DIR}/processors/doublethreshold.h
    ${MOD_DIR}/processors/gradientvectorflow.h
    ${MOD_DIR}/processors/itkprocessor.h
    ${MOD_DIR}/processors/volumefilter_itk.h
    ${MOD_DIR}/processors/valuedregionalmaximaimagefilter.h
    ${MOD_DIR}/processors/vesselness.h
    ${MOD_DIR}/processors/mutualinformationregistration.h
    ${MOD_DIR}/processors/fastmarchingimagefilter.h
    ${MOD_DIR}/processors/curveslevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/geodesicactivecontourlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/geodesicactivecontourshapepriorlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/narrowbandcurveslevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/narrowbandthresholdsegmentationlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/shapedetectionlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/watershedimagefilter.h
    ${MOD_DIR}/processors/thresholdlabelerimagefilter.h
    ${MOD_DIR}/processors/labelmapoverlayimagefilter.h
    ${MOD_DIR}/processors/labelmapcontouroverlayimagefilter.h
    ${MOD_DIR}/processors/bayesianclassifierimagefilter.h
)
