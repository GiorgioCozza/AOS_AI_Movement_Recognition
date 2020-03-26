




#include "LSM6DSL.h"



bool LSM6DSLAccGyr::init(){

	uint8_t n_bdu = 0;
	uint8_t n_fifo = 0;
	uint8_t n_odr = 0;
	
	
	if (!LSM6DSLAccGyr::io_read((uint8_t *)&n_bdu, LSM6DSL_ACC_GYRO_CTRL3_C, 1))
		return false;
		
	n_bdu &= ~LSM6DSL_BDU_MASK;
	n_bdu |= LSM6DSL_BDU_BLOCK_UPDATE;
	
	if (!LSM6DSLAccGyr::io_write((uint8_t *)&n_bdu, LSM6DSL_ACC_GYRO_CTRL3_C, 1))
		return false;
		
		
		
	if (!LSM6DSLAccGyr::io_read((uint8_t *)&n_fifo, LSM6DSL_ACC_GYRO_FIFO_CTRL5, 1))
		return false;
	
	n_fifo &= LSM6DSL_FIFO_MODE_MASK;
	n_fifo |= LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS;
	
	if (!LSM6DSLAccGyr::io_write((uint8_t *)&n_fifo, LSM6DSL_ACC_GYRO_FIFO_CTRL5, 1))
			return false;
			
			
			
	if(!set_acc_odr(104.0f))
		return false;
	
	
	if (!set_acc_fs(2.0f))
		return false;
		
	
	if (!set_gyr_odr(104.0f))
		return false;
		
	if (!set_gyr_fs(2000.0f))
		return false;
	
	return true;
}



bool LSM6DSLAccGyr::read_id(uint8_t * id){

	return LSM6DSLAccGyr::io_read((uint8_t *)&id, LSM6DSL_ACC_GYRO_WHO_AM_I_REG, 1);

}



bool LSM6DSLAccGyr::get_acc_axes(int32_t * aData){

	uint8_t tmp_val[6] = {0,0,0,0,0,0};
	int16_t raw_val[3] = {0,0,0};
	uint8_t k = 0, i = 0, j = 0;
	float * sens;
	
	for (i = 0; i < NUM_AXES; i++)
		for(j = 0; j < BYTES_PER_DIMENSION; j++)
			if (!LSM6DSLAccGyr::io_read((uint8_t *)(tmp_val+k), LSM6DSL_OUTX_L_XL+k),1)
				return false;
		
	aData[0] = ((((int16_t)raw_val[1]) << 8 ) + (int16_t)raw_val[0]);
	aData[1] = ((((int16_t)raw_val[3]) << 8 ) + (int16_t)raw_val[2]);
	aData[2] = ((((int16_t)raw_val[5]) << 8 ) + (int16_t)raw_val[4]);
	
	if (!LSM6DSLAccGyr::get_acc_sensitivity(sen))
		return false;
	
	aData[0] = (int32_t)(aData[0] * sens);
	aData[1] = (int32_t)(aData[1] * sens);
	aData[2] = (int32_t)(aData[2] * sens);
	
	return true;
}



bool LSM6DSLAccGyr::get_acc_sensitivity(float * aSens){
	
	uint8_t fscale = 0;
	
	
	if (!LSM6DSLAccGyr::io_read((uint8_t *)&fscale, LSM6DSL_CTRL1_XL, 1))
		return false;
	
	fscale &= LSM6DSL_FS_XL_MASK;
	
	switch(fscale) {

        case LSM6DSL_FS_XL_2g:
            *aSens = (float) LSM6DSL_ACC_SENSITIVITY_FOR_FS_2G;
            break;
        case LSM6DSL_FS_XL_4g:
            *aSens = (float) LSM6DSL_ACC_SENSITIVITY_FOR_FS_4G;
            break;
        case LSM6DSL_FS_XL_8g:
            *aSens = (float) LSM6DSL_ACC_SENSITIVITY_FOR_FS_8G;
            break;
        case LSM6DSL_FS_XL_16g:
            *aSens = (float) LSM6DSL_ACC_SENSITIVITY_FOR_FS_16G;
            break;
        default:
            *aSens = -1.0f;
            return false;
    }
	return true;
}



bool LSM6DSLAccGyr::get_acc_odr(float *aOdr){

    uint8_t n_odr = 0;

    if (!LSM6DSLAccGyr::io_read((uint8_t *)&n_odr, LSM6DSL_CTRL1_XL, 1))
        return false;

    n_odr &= LSM6DSL_ODR_XL_MASK;


    switch( n_odr )
    {
        case LSM6DSL_ODR_XL_POWER_DOWN:
            *aOdr = 0.0f;
            break;
        case LSM6DSL_ODR_XL_13Hz:
            *aOdr = 13.0f;
            break;
        case LSM6DSL_ODR_XL_26Hz:
            *aOdr = 26.0f;
            break;
        case LSM6DSL_ODR_XL_52Hz:
            *aOdr = 52.0f;
            break;
        case LSM6DSL_ODR_XL_104Hz:
            *aOdr = 104.0f;
            break;
        case LSM6DSL_ODR_XL_208Hz:
            *aOdr = 208.0f;
            break;
        case LSM6DSL_ODR_XL_416Hz:
            *aOdr = 416.0f;
            break;
        case LSM6DSL_ODR_XL_833Hz:
            *aOdr = 833.0f;
            break;
        case LSM6DSL_ODR_XL_1660Hz:
            *aOdr = 1660.0f;
            break;
        case LSM6DSL_ODR_XL_3330Hz:
            *aOdr = 3330.0f;
            break;
        case LSM6DSL_ODR_XL_6660Hz:
            *aOdr = 6660.0f;
            break;
        default:
            *aOdr = -1.0f;
            return false;
    }

    return true;
}



bool LSM6DSLAccGyr::get_acc_fs(float *aFs){

    uint8_t n_fs = 0;

    if (!LSM6DSLAccGyr::io_read((uint8_t)&n_fs, LSM6DSL_ACC_GYRO_CTRL1_XL, 1))
        return false;

    n_fs &= LSM6DSL_FS_XL_MASK;

    switch( n_fs ){

        case LSM6DSL_FS_XL_2g:
            *aFs = 2.0f;
            break;
        case LSM6DSL_FS_XL_4g:
            *aFs = 4.0f;
            break;
        case LSM6DSL_FS_XL_8g:
            *aFs = 8.0f;
            break;
        case LSM6DSL_FS_XL_16g:
            *aFs = 16.0f;
            break;
        default:
            *aFs = -1.0f;
            return false;
    }

    return true;
}


bool LSM6DSLAccGyr::set_acc_fs(float aFs){

    uint8_t n_fs = 0, o_fs = 0;

    if ( aFs <= 2.0f )
        n_fs = LSM6DSL_FS_XL_2g;
    else if ( aFs <= 4.0f )
        n_fs = LSM6DSL_FS_XL_4g;
    else if ( aFs <= 8.0f )
        n_fs = LSM6DSL_FS_XL_8g;
    else
        n_fs = LSM6DSL_FS_XL_16g;

    if (!LSM6DSLAccGyr::io_read(((uint8_t *)&o_fs, LSM6DSL_CTRL1_XL, 1)))
        return false;

    o_fs &= ~LSM6DSL_CTRL1_XL;
    o_fs |= n_fs;

    if (!LSM6DSLAccGyr::io_write(((uint8_t *)&o_fs, LSM6DSL_CTRL1_XL, 1)))
        return false;

    return true;
}



bool LSM6DSLAccGyr::get_gyr_axes(int32_t * gData){

    uint8_t tmp_val[6] = {0,0,0,0,0,0};
    int16_t raw_val[3] = {0,0,0};
    uint8_t k = 0, i = 0, j = 0;
    float * sens;

    for (i = 0; i < NUM_AXES; i++)
        for(j = 0; j < BYTES_PER_DIMENSION; j++)
            if (!LSM6DSLAccGyr::io_read((uint8_t *)(tmp_val+k), LSM6DSL_OUTX_L_G+k),1)
                return false;

    gData[0] = ((((int16_t)raw_val[1]) << 8 ) + (int16_t)raw_val[0]);
    gData[1] = ((((int16_t)raw_val[3]) << 8 ) + (int16_t)raw_val[2]);
    gData[2] = ((((int16_t)raw_val[5]) << 8 ) + (int16_t)raw_val[4]);

    if (!LSM6DSLAccGyr::get_acc_sensitivity(sen))
        return false;

    gData[0] = (int32_t)(aData[0] * sens);
    gData[1] = (int32_t)(aData[1] * sens);
    gData[2] = (int32_t)(aData[2] * sens);

    return true;
}



bool LSM6DSLAccGyr::get_gyr_sensitivity(float * aSens){

    uint8_t fscale125 = 0;
    uint8_t fscale = 0;

    if (!LSM6DSLAccGyr::io_read((uint8_t *)&fscale125, LSM6DSL_CTRL2_G, 1))
        return false;

    if (fscale125 == LSM6DSL_FS_125_ENABLED)
        *aSens = (float)LSM6DSL_GYRO_SENSITIVITY_FOR_FS_125DPS;


    if (!LSM6DSLAccGyr::io_read((uint8_t *)&fscale, LSM6DSL_CTRL2_G, 1))
        return false;

    fscale &= LSM6DSL_FS_XL_MASK;

    switch(fscale) {
        case LSM6DSL_ACC_GYRO_FS_G_245dps:
            *aSens = ( float )LSM6DSL_GYRO_SENSITIVITY_FOR_FS_245DPS;
            break;
        case LSM6DSL_ACC_GYRO_FS_G_500dps:
            *aSens = ( float )LSM6DSL_GYRO_SENSITIVITY_FOR_FS_500DPS;
            break;
        case LSM6DSL_ACC_GYRO_FS_G_1000dps:
            *aSens = ( float )LSM6DSL_GYRO_SENSITIVITY_FOR_FS_1000DPS;
            break;
        case LSM6DSL_ACC_GYRO_FS_G_2000dps:
            *aSens = ( float )LSM6DSL_GYRO_SENSITIVITY_FOR_FS_2000DPS;
            break;
        default:
            *aSens = -1.0f;
            return 1;
    }
    return true;
}



bool LSM6DSLAccGyr::get_gyr_odr(float * gOdr){

    uint8_t n_odr = 0;

    if (!LSM6DSLAccGyr::io_read((uint8_t *)&n_odr, LSM6DSL_ACC_GYRO_CTRL2_G, 1))
        return false;

    n_odr &= LSM6DSL_ODR_G_MASK;

    switch( n_odr )
    {
        case LSM6DSL_ODR_G_POWER_DOWN:
            *gOdr = 0.0f;
            break;
        case LSM6DSL_ODR_G_13Hz:
            *gOdr = 13.0f;
            break;
        case LSM6DSL_ODR_G_26Hz:
            *gOdr = 26.0f;
            break;
        case LSM6DSL_ODR_G_52Hz:
            *gOdr = 52.0f;
            break;
        case LSM6DSL_ODR_G_104Hz:
            *gOdr = 104.0f;
            break;
        case LSM6DSL_ODR_G_208Hz:
            *gOdr = 208.0f;
            break;
        case LSM6DSL_ODR_G_416Hz:
            *gOdr = 416.0f;
            break;
        case LSM6DSL_ODR_G_833Hz:
            *gOdr = 833.0f;
            break;
        case LSM6DSL_ODR_G_1660Hz:
            *gOdr = 1660.0f;
            break;
        case LSM6DSL_ODR_G_3330Hz:
            *gOdr = 3330.0f;
            break;
        case LSM6DSL_ODR_G_6660Hz:
            *gOdr = 6660.0f;
            break;
        default:
            *gOdr = -1.0f;
            return false;
    }

    return true;

}



bool LSM6DSLAccGyr::set_gyr_fs(float gFs){

    uint8_t n_fs = 0, o_fs = 0;
    uint8_t n_fs125 = 0, o_fs125 = 0;

    if ( gFs <= 125.0f ) {
        if (!LSM6DSLAccGyr::io_read((uint8_t * ) & o_fs125, LSM6DSL_CTRL2_G, 1))
            return false;

        o_fs125 &= ~LSM6DSL_FS_G_MASK;
        o_fs125 |= LSM6DSL_FS_125_ENABLED;

        if (!LSM6DSLAccGyr::io_write((uint8_t * ) & o_fs125, LSM6DSL_CTRL2_G, 1))
            return false;
    }else{

        if ( gFs <= 245.0f )
            n_fs = LSM6DSL_FS_G_245dps;
        else if ( gFs <= 500.0f )
            n_fs = LSM6DSL_FS_G_500dps;
        else if ( gFs <= 1000.0f )
            n_fs = LSM6DSL_FS_G_1000dps;
        else
            n_fs = LSM6DSL_FS_G_2000dps;


        if (!LSM6DSLAccGyr::io_read((uint8_t * ) & o_fs125, LSM6DSL_CTRL2_G, 1))
            return false;

        o_fs125 &= ~LSM6DSL_FS_G_MASK;
        o_fs125 |= LSM6DSL_FS_125_DISABLED;

        if (!LSM6DSLAccGyr::io_write((uint8_t * ) & o_fs125, LSM6DSL_CTRL2_G, 1))
            return false;


        if (!LSM6DSLAccGyr::io_read((uint8_t * ) & o_fs, LSM6DSL_CTRL2_G, 1))
            return false;

        o_fs &= ~LSM6DSL_FS_G_MASK;
        o_fs |= LSM6DSL_FS_125_DISABLED;

        if (!LSM6DSLAccGyr::io_write((uint8_t * ) & o_fs, LSM6DSL_CTRL2_G, 1))
            return false;

    }
}



bool LSM6DSLAccGyr::set_gyr_odr(float gOdr) {

    uint8_t n_odr = 0, o_odr = 0;

    if ( gOdr <= 13.0f )
        n_odr = LSM6DSL_ODR_XL_13Hz;
    else if ( gOdr <= 26.0f )
        n_odr = LSM6DSL_ODR_XL_26Hz;
    else if ( gOdr <= 52.0f )
        n_odr = LSM6DSL_ODR_XL_52Hz;
    else if ( gOdr <= 104.0f )
        n_odr = LSM6DSL_ODR_XL_104Hz;
    else if ( gOdr <= 208.0f )
        n_odr = LSM6DSL_ODR_XL_208Hz;
    else if ( gOdr <= 416.0f )
        n_odr = LSM6DSL_ODR_XL_416Hz;
    else if ( gOdr <= 833.0f )
        n_odr = LSM6DSL_ODR_XL_833Hz;
    else if ( gOdr <= 1660.0f )
        n_odr = LSM6DSL_ODR_XL_1660Hz;
    else if ( gOdr <= 3330.0f )
        n_odr = LSM6DSL_ODR_XL_3330Hz;
    else
        n_odr = LSM6DSL_ODR_XL_6660Hz;

    if (!LSM6DSLAccGyr::io_read((uint8_t *)&o_odr, LSM6DSL_CTRL2_G, 1))
        return false;

    o_odr &= ~LSM6DSL_ODR_G_MASK;
    o_odr |= n_odr;

    if (!LSM6DSLAccGyr::io_write((uint8_t *)&o_odr, LSM6DSL_CTRL2_G, 1))
        return false;

    return true;
}