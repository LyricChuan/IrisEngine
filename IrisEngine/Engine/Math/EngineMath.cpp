#include "EngineMath.h"

namespace EngineMath
{
	XMFLOAT4X4 IdentityMatrix4x4()
	{
		return XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
    XMFLOAT4 ToFloat4(const fvector_4d& InV4d)
    {
        return XMFLOAT4(InV4d.x, InV4d.y, InV4d.z,InV4d.w);
    }
    XMFLOAT3 ToFloat3(const fvector_3d& InV3d)
    {
        return XMFLOAT3(InV3d.x, InV3d.y, InV3d.z);
    }

    fvector_3d ToVector3d(const XMFLOAT3& InV3d)
    {
        return fvector_3d(InV3d.x, InV3d.y, InV3d.z);
    }

    bool IsAngleRange(float InAngle, float X, float Y)
    {
        if (InAngle >= min(X, Y))
        {
            if (InAngle <= max(X, Y))
            {
                return true;
            }
        }

        return false;
    }

    fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint)
    {
        float AnglePre = (180.f / 3.1415926f);

        float R = sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y + InPoint.z * InPoint.z);
        float Theta = acos(InPoint.y / R) * AnglePre;
        float Phi = atan2f(InPoint.z, InPoint.x) * AnglePre;

        return fvector_3d(R, Theta, Phi);
    }

    struct FCubeMapAxialRangeR
    {
        FCubeMapAxialRangeR()
            :PositiveX(45.f, 135.f, 45.f, -45.f)//phi ���� 0��45��  0��-45��
            ,NegativeX(45.f, 135.f, 135.f, -135.f)//phi ���� 135��180�� -135��-180��
            ,PositiveY(0.f,  45.f,360.f,-360.f)//theta ���� 0��45��
            ,NegativeY(135.f, 180.f, 360.f, -360.f)//theta ���� 135��180��
            ,PositiveZ(45.f,135.f, 45.f, 135.f)
            ,NegativeZ(45.f, 135.f,-45.f, -135.f)
        {}

        const fvector_4d PositiveX;
        const fvector_4d NegativeX;
                           
        const fvector_4d PositiveY;
        const fvector_4d NegativeY;
                            
        const fvector_4d PositiveZ;
        const fvector_4d NegativeZ;
    };

    bool IsAngleAxisRange(
        float InAngle,
        float X,
        float Y, 
        float InCriticalValue, 
        bool bComMin)
    {
        if (bComMin) //��С
        {
            if (InAngle > InCriticalValue)
            {
                if (InAngle <= max(X,Y))
                {
                    return true;
                }
            }
            else if (InAngle < InCriticalValue)
            {
                if (InAngle >= min(X, Y))
                {
                    return true;
                }
            }
            else if(InAngle == 0.f)
            {
                return true;
            }
        }
        else
        {
            if (InAngle > 0.f) //+
            {
                if (InAngle >= max(X, Y))
                {
                    if (InAngle <= InCriticalValue)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (InAngle <= min(X, Y))
                {
                    if (InAngle >= -InCriticalValue)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool IsPointInCubeMapVieport(
        float InPointTheta,
        float InPointPhi,
        fvector_4d InCompareValue, 
        int bAxisRange = 0,
        float InCriticalValue = 0.f, 
        bool bComMin = false)
    {
        if (IsAngleRange(InPointTheta, InCompareValue.x,InCompareValue.y))
        {
            if (bAxisRange == 0)
            {
                if (IsAngleAxisRange(InPointPhi, InCompareValue.z, InCompareValue.w, InCriticalValue, bComMin))
                {
                    return true;
                }
            }
            else if (bAxisRange == 1)
            {
                if (InPointPhi >= InCompareValue.z)
                {
                    if (InPointPhi <= InCompareValue.w)
                    {
                        return true;
                    }
                }
            }
            else if (bAxisRange == 2)
            {
                if (InPointPhi <= InCompareValue.z)
                {
                    if (InPointPhi >= InCompareValue.w)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (IsAngleRange(InPointPhi, InCompareValue.z, InCompareValue.w))
                {
                    return true;
                }
            }
        }

        return false;
    }

    ECubeMapFaceType GetSampleCubeMapIndexR(const fvector_3d& InPointPosition)
    {
        static FCubeMapAxialRangeR CubeMapAxialRangeRight;

        //ȷ�����Ѿ�תΪCubeMapViewport�µ�����
        //תΪ��������
        fvector_3d Point = GetPointSphericalCoordinates(InPointPosition);

        //��������ֵ
        float PointTheta = Point.y;
        float PointPhi = Point.z;

        if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.PositiveX,3,0.f,true))
        {
            return ECubeMapFaceType::POSITIVE_X;
        }
        else if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.NegativeX,0, 180.f,false))
        {
            return ECubeMapFaceType::NEGATIVE_X;
        }
        else if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.PositiveY, 3))
        {
            return ECubeMapFaceType::POSITIVE_Y;
        }
        else if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.NegativeY, 3))
        {
            return ECubeMapFaceType::NEGATIVE_Y;
        }
        else if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.PositiveZ,1))
        {
            return ECubeMapFaceType::POSITIVE_Z;
        }
        else if (IsPointInCubeMapVieport(PointTheta, PointPhi, CubeMapAxialRangeRight.NegativeZ, 2))
        {
            return ECubeMapFaceType::NEGATIVE_Z;
        }

        return ECubeMapFaceType::NEGATIVE_INVALID;
    }
}