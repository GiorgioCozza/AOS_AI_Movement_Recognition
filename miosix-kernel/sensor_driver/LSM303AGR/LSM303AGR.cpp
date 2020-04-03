


#include "LSM303AGR.h"



bool LSM303AGRAccMag::init(){

    uint8_t a_bdu = 0, m_bdu = 0;
    uint8_t a_fifo = 0;
    uint8_t a_pm = 0;

    uint8_t m_md = 0;

    /* Set to No-FIFO buffering */
    a_fifo |= LSM303AGR_ACC_BYPASS;
    if (!LSM303AGRAccMag::io_write((uint8_t *)&a_fifo, LSM303AGR_ACC_FIFO_CTRL, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FIFO_MASK))
        return false;


    // ACCELEROMETER INITIALIZATION

    /* Set BLOCK DATA UPDATE to DISABLED: continuous update*/
    a_bdu |= LSM303AGR_ACC_BDU_DISABLED;
    if (!LSM303AGRAccMag::io_write((uint8_t *)&a_bdu, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_BDU_MASK))
        return false;

    /* Set Full Scale for the accelerometer*/
    if (!set_acc_fs( C_LSM303AGR_ACC_FS ))
        return false;

    /* Set the POWER MODE valid for all the sensor axes*/
    a_pm |= LSM303AGR_CTRL1_XEN_YEN_ZEN_ENABLED;
    if (!LSM303AGRAccMag::io_write((uint8_t *)&a_pm, LSM303AGR_ACC_CTRL1, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_CTRL1_XEN_YEN_ZEN_MASK))
        return false;

    /* Set Output Data Rate (accelerometer) */
    if (!set_acc_odr( C_LSM303AGR_ACC_ODR ))
        return false;



    // MAGNETOMETER INITIALIZATION

    /* Set System-mode to CONTINUOUS: the sensor continuously performs read and update data register */
    m_md |= LSM303AGR_MAG_MD_CONTINUOS;
    if (!LSM303AGRAccMag::io_write((uint8_t *)&m_md, LSM303AGR_MAG_CFG_A, 1, LSM303AGR_MAG_I2C_ADDRESS, LSM303AGR_MAG_MD_MASK))
        return false;

    /* Set BLOCK DATA UPDATE to DISABLED */
    m_bdu |= LSM303AGR_MAG_BDU_DISABLED;
    if (!LSM303AGRAccMag::io_write((uint8_t *)&m_bdu, LSM303AGR_MAG_CFG_C, 1, LSM303AGR_MAG_I2C_ADDRESS, LSM303AGR_MAG_BDU_MASK))
        return false;

    /* Set Output Data Rate (magnetometer)*/
    if ( !set_mag_odr( C_LSM303AGR_MAG_ODR ) )
        return false;

    return true;
}



bool LSM303AGRAccMag::read_acc_id(uint8_t * id){

    return LSM303AGRAccMag::io_read((uint8_t *)id, LSM303AGR_ACC_WHO_AM_I_ADDR, 1, LSM303AGR_ACC_I2C_ADDRESS);

}



bool LSM303AGRAccMag::get_acc_axes(int32_t * aData){

    uint8_t tmp_val[6] = {0,0,0,0,0,0};
    int16_t raw_val[3] = {0,0,0};
    uint8_t a_lp = 0, a_hr = 0, shift = 0;
    float sens;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_lp, LSM303AGR_ACC_CTRL1, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_LP_MASK))
        return false;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_hr, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_HR_MASK))
        return false;

    if ( a_lp == LSM303AGR_ACC_LP_ENABLED && a_hr == LSM303AGR_ACC_HR_DISABLED ) {
        shift = 8;
    }else if ( a_lp == LSM303AGR_ACC_LP_DISABLED && a_hr == LSM303AGR_ACC_HR_DISABLED ) {
        shift = 6;
    }else if ( a_lp == LSM303AGR_ACC_LP_DISABLED && a_hr == LSM303AGR_ACC_HR_ENABLED ) {
        shift = 4;
    }else
        return false;

    if (!get_acc_sensitivity(&sens))
        return false;

    sens *= 1000.0f;

    if (!LSM303AGRAccMag::io_read((uint8_t *)tmp_val, LSM303AGR_ACC_OUT_X_L,6, LSM303AGR_ACC_I2C_ADDRESS, 0xFF))
        return false;


    raw_val[0] = (((((int16_t)tmp_val[1]) << 8 ) + (int16_t)tmp_val[0]));
    raw_val[1] = (((((int16_t)tmp_val[3]) << 8 ) + (int16_t)tmp_val[2]));
    raw_val[2] = (((((int16_t)tmp_val[5]) << 8 ) + (int16_t)tmp_val[4]));

    raw_val[0] = (((raw_val[0] >> shift) * sens + 500)) / 1000;
    raw_val[1] = (((raw_val[1] >> shift) * sens + 500)) / 1000;
    raw_val[2] = (((raw_val[2] >> shift) * sens + 500)) / 1000;


    aData[0] = (int32_t)(raw_val[0]);
    aData[1] = (int32_t)(raw_val[1]);
    aData[2] = (int32_t)(raw_val[2]);

    return true;
}



bool LSM303AGRAccMag::get_acc_sensitivity(float * aSens){

    uint8_t a_lp = 0;
    uint8_t a_hr = 0;
    uint8_t a_fs = 0;


    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_lp, LSM303AGR_ACC_CTRL1, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_LP_MASK))
        return false;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_hr, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_HR_MASK))
        return false;

    if ( a_lp == LSM303AGR_ACC_LP_DISABLED && a_hr == LSM303AGR_ACC_HR_DISABLED ){

        if (!LSM303AGRAccMag::io_read((uint8_t *)&a_fs, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FS_MASK))
            return false;

        switch( a_fs ){

            case LSM303AGR_ACC_FS_2G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_2G_NORMAL;
                break;

            case LSM303AGR_ACC_FS_4G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_4G_NORMAL;
                break;

            case LSM303AGR_ACC_FS_8G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_8G_NORMAL;
                break;

            case LSM303AGR_ACC_FS_16G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_16G_NORMAL;
                break;
        }

    } else if ( a_lp == LSM303AGR_ACC_LP_ENABLED && a_hr == LSM303AGR_ACC_HR_DISABLED ){

        if (!LSM303AGRAccMag::io_read((uint8_t *)&a_fs, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FS_MASK))
            return false;

        switch( a_fs ){

            case LSM303AGR_ACC_FS_2G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_2G_LOW_POWER;
                break;

            case LSM303AGR_ACC_FS_4G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_4G_LOW_POWER;
                break;

            case LSM303AGR_ACC_FS_8G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_8G_LOW_POWER;
                break;

            case LSM303AGR_ACC_FS_16G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_16G_LOW_POWER;
                break;
        }

    } else if ( a_lp == LSM303AGR_ACC_LP_DISABLED && a_hr == LSM303AGR_ACC_HR_ENABLED ){

        if (!LSM303AGRAccMag::io_read((uint8_t *)&a_fs, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FS_MASK))
            return false;

        switch( a_fs ){

            case LSM303AGR_ACC_FS_2G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_2G_HIGH_RES;
                break;

            case LSM303AGR_ACC_FS_4G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_4G_HIGH_RES;
                break;

            case LSM303AGR_ACC_FS_8G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_8G_HIGH_RES;
                break;

            case LSM303AGR_ACC_FS_16G:
                *aSens = LSM303AGR_X_SENSITIVITY_FS_16G_HIGH_RES;
                break;
        }

    } else {
        // high resolution and low power mode, not allowed!
        return false;

    }

    return true;
}



bool LSM303AGRAccMag::get_acc_odr(float *aOdr){

    uint8_t n_odr = 0;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&n_odr, LSM303AGR_ACC_CTRL1, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_ODR_MASK))
        return false;


    switch( n_odr )
    {
        case LSM303AGR_ACC_ODR_POWER_DOWN:
            *aOdr = 0.0f;
            break;
        case LSM303AGR_ACC_ODR_1HZ:
            *aOdr = 1.0f;
            break;
        case LSM303AGR_ACC_ODR_10HZ:
            *aOdr = 10.0f;
            break;
        case LSM303AGR_ACC_ODR_25HZ:
            *aOdr = 25.0f;
            break;
        case LSM303AGR_ACC_ODR_50HZ:
            *aOdr = 50.0f;
            break;
        case LSM303AGR_ACC_ODR_100HZ:
            *aOdr = 100.0f;
            break;
        case LSM303AGR_ACC_ODR_200HZ:
            *aOdr = 200.0f;
            break;
        case LSM303AGR_ACC_ODR_400HZ:
            *aOdr = 400.0f;
            break;
        default:
            *aOdr = -1.0f;
            return false;
    }

    return true;
}



bool LSM303AGRAccMag::get_acc_fs(float *aFs){

    uint8_t a_fs = 0;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_fs, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FS_MASK))
        return false;


    switch( a_fs ){

        case LSM303AGR_ACC_FS_2G:
            *aFs = 2.0f;
            break;
        case LSM303AGR_ACC_FS_4G:
            *aFs = 4.0f;
            break;
        case LSM303AGR_ACC_FS_8G:
            *aFs = 8.0f;
            break;
        case LSM303AGR_ACC_FS_16G:
            *aFs = 16.0f;
            break;
        default:
            *aFs = -1.0f;
            return false;
    }

    return true;
}



bool LSM303AGRAccMag::set_acc_fs(float aFs){

    uint8_t a_fs = 0;

    if ( aFs <= 2.0f )
        a_fs = LSM303AGR_ACC_FS_2G;
    else if ( aFs <= 4.0f )
        a_fs = LSM303AGR_ACC_FS_4G;
    else if ( aFs <= 8.0f )
        a_fs = LSM303AGR_ACC_FS_8G;
    else
        a_fs = LSM303AGR_ACC_FS_16G;


    if (!LSM303AGRAccMag::io_write((uint8_t *)&a_fs, LSM303AGR_ACC_CTRL4, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_FS_MASK))
        return false;

    return true;
}



bool LSM303AGRAccMag::set_acc_odr(float aOdr) {

    uint8_t n_odr = 0;

    if ( aOdr <= 1.0f )
        n_odr = LSM303AGR_ACC_ODR_1HZ;
    else if ( aOdr <= 10.0f )
        n_odr = LSM303AGR_ACC_ODR_10HZ;
    else if ( aOdr <= 25.0f )
        n_odr = LSM303AGR_ACC_ODR_25HZ;
    else if ( aOdr <= 50.0f )
        n_odr = LSM303AGR_ACC_ODR_50HZ;
    else if ( aOdr <= 100.0f )
        n_odr = LSM303AGR_ACC_ODR_100HZ;
    else if ( aOdr <= 200.0f )
        n_odr = LSM303AGR_ACC_ODR_200HZ;
    else
        n_odr = LSM303AGR_ACC_ODR_400HZ;


    if (!LSM303AGRAccMag::io_write((uint8_t *)&n_odr, LSM303AGR_ACC_CTRL1, 1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_ODR_MASK))
        return false;

    return true;
}



bool LSM303AGRAccMag::read_mag_id(uint8_t * id){

    return LSM303AGRAccMag::io_read((uint8_t *)id, LSM303AGR_MAG_WHO_AM_I_ADDR, 1, LSM303AGR_MAG_I2C_ADDRESS);

}



bool LSM303AGRAccMag::get_mag_axes(int32_t *mData){

    uint8_t tmp_val[6] = {0,0,0,0,0,0};
    int16_t *raw_val;
    float sens;


    if (!LSM303AGRAccMag::io_read((uint8_t * )tmp_val, LSM303AGR_MAG_OUTX_L, 6, LSM303AGR_MAG_I2C_ADDRESS, 0xFF))
        return false;


    raw_val = (int16_t *)tmp_val;

    if (!LSM303AGRAccMag::get_mag_sensitivity(&sens))
        return false;

    mData[0] = (int32_t)(raw_val[0] * sens);
    mData[1] = (int32_t)(raw_val[1] * sens);
    mData[2] = (int32_t)(raw_val[2] * sens);

    return true;
}



bool LSM303AGRAccMag::get_mag_sensitivity(float * mSens){

    // Magnetometer sensitivity can only be 1.5f (see full scale)
    *mSens = LSM303AGR_MAG_SENSITIVITY_VAL;
    return true;
}



bool LSM303AGRAccMag::get_mag_odr(float * mOdr){

    uint8_t n_odr = 0;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&n_odr, LSM303AGR_MAG_CFG_A, 1, LSM303AGR_MAG_I2C_ADDRESS, LSM303AGR_MAG_ODR_MASK))
        return false;


    switch( n_odr )
    {
        case LSM303AGR_MAG_ODR_10HZ:
            *mOdr = 10.0f;
            break;
        case LSM303AGR_MAG_ODR_20HZ:
            *mOdr = 20.0f;
            break;
        case LSM303AGR_MAG_ODR_50HZ:
            *mOdr = 50.0f;
            break;
        case LSM303AGR_MAG_ODR_100HZ:
            *mOdr = 100.0f;
            break;
        default:
            *mOdr = -1.0f;
            return false;
    }

    return true;

}



bool LSM303AGRAccMag::set_mag_odr(float mOdr){

    uint8_t n_odr = 0;


    if ( mOdr <= 10.0f )
        n_odr = LSM303AGR_MAG_ODR_10HZ;
    else if ( mOdr <= 20.0f )
        n_odr = LSM303AGR_MAG_ODR_20HZ;
    else if ( mOdr <= 50.0f )
        n_odr = LSM303AGR_MAG_ODR_50HZ;
    else
        n_odr = LSM303AGR_MAG_ODR_100HZ;

    if (!LSM303AGRAccMag::io_write((uint8_t *)&n_odr, LSM303AGR_MAG_CFG_A, 1, LSM303AGR_MAG_I2C_ADDRESS, LSM303AGR_MAG_ODR_MASK))
        return false;

    return true;

}



bool LSM303AGRAccMag::get_mag_fs(float *mFs){

    // Magnetometer has 1 full scale mode
    *mFs = LSM303AGR_MAG_FS_VAL;

    return true;
}

