/** @breif Gh control functions
 *  @file ghcontrol.c
 */
#include <stdio.h>
#include "ghcontrol.h"

/** @brief Prints the program title
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param sname
 *  @return void
 */

void GhDisplayHeader (const char * sname)
{
	fprintf(stdout,"%s's CENG Greenhouse Controller",sname);
}

/** @brief Displays the GhDisplayReader function
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return void
 */

void GhControllerInit(void)
{
	srand((unsigned)time(NULL));
	GhDisplayHeader("Joshua Dedeigbo");

#if SENSEHAT
	ShInit();
#endif
}

/** @brief displays the values collected from the GhGetReadings function
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param readings rdata
 *  @return void
 */

void GhDisplayReadings(struct readings rdata)
{
	printf("\nReadings\tT: %4.1lfC\tH: %4.1lf%%\tP: %6.1lfmB\n",
	ctime(&rdata.rtime),rdata.temperature,rdata.humidity,
	rdata.pressure);
}

/** @brief displays target readings for temperature and humidity
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return void
 */

void GhDisplayTargets(struct setpoints spts)
{
	fprintf(stdout,"Targets \tT: %5.1lfC\tH: %5.1lf%%\n",STEMP,SHUMID);
}

/** @brief gets serial number
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return serial
 */

uint64_t GhGetSerial(void)
{
static uint64_t serial = 0;
FILE * fp;
char buf[SYSINFOBUFSZ];
char searchstring[] = SEARCHSTR;
fp = fopen ("/proc/cpuinfo", "r");
if (fp != NULL)
{
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		if (!strncasecmp(searchstring, buf, strlen(searchstring)))
		{
			sscanf(buf+strlen(searchstring), "%Lx", &serial);
		}
	}
	fclose(fp);
}
if(serial==0)
{
	system("uname -a");
	system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
	fp = fopen ("stamp.txt", "r");
	if (fp != NULL)
	{
		while (fgets(buf, sizeof(buf), fp) != NULL)
		{
			sscanf(buf, "%Lx", &serial);
		}
		fclose(fp);
	}
}
	return serial;
}

/** @brief putting the all of the functions into variables
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return readings structure
 */

struct readings GhGetReadings(void)
{
	struct readings now = {0};

	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
       	now.humidity = GhGetHumidity();
       	now.pressure = GhGetPressure();
		return now;
}

/** @brief gets the value of temperature
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return double
 */

double GhGetTemperature(void)
{
#if SIMTEMPERATURE
	return GhGetRandom(USTEMP-LSTEMP)+LSTEMP;
#else
		ht221sData_s ct = {0};

		ct = ShGetHT221SData();
	return ct.temperature;
#endif

}

/** @brief gets the value of humidity
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return double
 */

double GhGetHumidity(void)
{
#if SIMHUMIDITY
	return GhGetRandom(USHUMID-LSHUMID)+LSHUMID;
#else

	ht221sData_s ct = {0};

		ct = ShGetHT221SData();
	return ct.humidity;
#endif
}

/** @brief gets the value of pressure
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return double
 */

double GhGetPressure(void)
{
#if SIMPRESSURE
	return GhGetRandom(USPRESS-LSPRESS)+LSPRESS;
#else
	lps25hData_s ct = {0};

		ct = ShGetLPS25HData();
	return ct.pressure;
#endif
}

/** @brief Setting the controls function for the heater and humidifier
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @return setpoints structure
 */

struct setpoints GhSetTargets(void)
{
	struct setpoints cpoints = {0};
	cpoints = GhRetrieveSetpoints("setpoints.dat");

	if(cpoints.temperature == 0)
	{
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	GhSaveSetpoints("setpoints.dat",cpoints);
	}
	return cpoints;
}


/** @brief Displays the controls function
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param heater
 *  @param humidifier
 *  @return setpoints structure
 */

void GhDisplayControls(struct controls ctrl)
{
	fprintf(stdout,"Controls\tHeater: %i\tHumidifier: %i\n", ctrl.heater, ctrl.humidifier);
}

/** @brief retrieves random number
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param range
 *  @return int
 */

int GhGetRandom(int range)
{
	return rand() % range;
}

/** @brief Delays program while loop
 * @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param milliseconds
 *  @return void
 */

void GhDelay(int milliseconds)
{
	long wait;
	clock_t now,start;
	
	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait )
	{
		now = clock();
	}
}

/** @brief ON and OFF control switch for the heater and humidifier
 *  @version 15FEB2021
 *  @author JoshuaDedeigbo
 *  @param target, rdata
 *  @return controls structure
 */

struct controls GhSetControls(struct setpoints target,struct readings rdata)
{
   	struct controls cset = {0};

	if(rdata.temperature<STEMP){cset.heater = ON;}
	else{cset.heater = OFF;}

	if(rdata.humidity< SHUMID){cset.humidifier = ON;}
	else {cset.humidifier = OFF;}
		return cset;
}

/** @brief stores the reading data
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param fname, ghdata
 *  @return int
 */


int GhLogData(char * fname, struct readings ghdata)
{
	FILE * fp;
	char ltime[CTIMESTRSZ];

	fp = fopen(fname, "a");	
	if (fp == NULL) {return 0;}
	strcpy(ltime, ctime(&ghdata.rtime));
	ltime[3] = ',';
	ltime[7] = ',';
	ltime[10] = ',';
	ltime[19] = ',';

	fprintf(fp,"\n%.24s,%5.1lf,%5.1lf,%6.1lf",ltime,ghdata.temperature,
	ghdata.humidity,ghdata.pressure);
	
	fclose(fp);
		return 1;

}

/** @brief saves the structure data inside the file
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param fname, spts
 *  @return int
 */

int GhSaveSetpoints(char * fname, struct setpoints spts)
{
	FILE * fp;
	fp = fopen(fname, "w");
	if (fp == NULL) 
	{
		return 0;
	}
	else
	{
	fwrite(&spts,sizeof(struct setpoints),1,fp);
	fclose(fp);
		return 1;
	}

}

/** @brief stores the reading data in spts
 *  @version 19FEB2021
 *  @author Joshua Dedeigbo
 *  @param fname
 *  @return setpoints structure
 */

struct setpoints GhRetrieveSetpoints(char * fname)
{
	struct setpoints spts = {0};
	
	FILE * fp;
	fp = fopen(fname, "r");
	if(fp == NULL)
	{
		return spts;
	}
	else
	{
	fwrite(&spts,sizeof(struct setpoints),1,fp);
	fclose(fp);
		return spts;
	}


}
void GhDisplayAll(struct readings rd, struct setpoints sd)
{
	int rv,sv,avh,avl;
	fbpixel_s pxc = {0};
	ShClearMatrix();

	//Temperature display
	rv = (8.0 * (((rd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
	sv = (8.0 * (((sd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;

	pxc.red = 0x00;
	pxc.green = 0xFF; 
	pxc.blue = 0x00;
	ShSetVerticalBar(TBAR,pxc,rv);
	pxc.red = 0xF0;
	pxc.green = 0x0F;
	pxc.blue = 0xF0;
	ShSetPixel(TBAR,sv,pxc);

	//Humidity display
	rv = (8.0 * (((rd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
	sv = (8.0 * (((sd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;

	pxc.red = 0x00;
	pxc.green = 0xFF; 
	pxc.blue = 0x00;
	ShSetVerticalBar(HBAR,pxc,rv);
	pxc.red = 0xF0;
	pxc.green = 0x0F;
	pxc.blue = 0xF0;
	ShSetPixel(HBAR,sv,pxc);

	//Pressure display
	rv = (8.0 * (((rd.pressure-LSPRESS)/(USPRESS-LSPRESS))+0.05))-1.0;
	
	pxc.red = 0x00;
	pxc.green = 0xFF; 
	pxc.blue = 0x00;
	ShSetVerticalBar(PBAR,pxc,rv);

}

