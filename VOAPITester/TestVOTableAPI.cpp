/*
 * Persistent Systems Private Limited, Pune, India (website - http://www.pspl.co.in)
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that this
 * copyright notice and the following paragraph appears in all
 * copies of this software.
 *
 * DISCLAIMER OF WARRANTY.
 * -----------------------
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 * IN NO EVENT SHALL THE RIGHTHOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "XPathHelper.h"
#include "VTable.h"
#include "global.h"
#include "MissingValueException.h"
#include "DatatypeMismatchException.h"


#include <iostream>
using namespace std;

/* function definitions */
void printStreamAttributes(Stream &s);
void printBinaryStream(Stream &s);
void printFitsData(FitsData &fd);
void printBinaryData(BinaryData &bd);
void printVTable(VTable &v);
void printResource(Resource &r);
void printGroup(Group &g, int groupNum);
void printFieldAttributes(Field &f, int fieldNum);
void printFieldRefAttributes(FieldRef &f, int fieldNum);
void printTableData(TableData &td, vector<Field> &fieldList);
void printLinkAttributes(Link &l, int linkNum);
void printValue(Values &v, int valueNum);
void printRange(Range &r);
void printOption(Option &o, int optionNum);
void printInfoAttributes(Info &f, int fieldNum);
void printParamAttributes(Param &f, int fieldNum);
void printParamRefAttributes(ParamRef &f, int fieldNum);
void printCoosysAttributes(Coosys &f, int fieldNum);
void testVTable(const char*	argv[]);
void testResource(const char* argv[]);
void printBits(char c, int numOfBits);
/* end of function definitions */


/*
* This is a test program to test the VOTable API.
*/
int
main(
			int				argc,
			const char*		argv[])
{
	
	if (argc != 3)
	{
		cout << "Usage TestVOTableAPI xmlfilepath xpath" << endl;
		return 0;
	}

	//testVTable(argv);
	testResource(argv);
	
	cout << endl << "Completed operation - exiting now." << endl;

	return 0;
}

/*
* This fuction tests the VTable by printing values in it.
*/
void testVTable(const char*	argv[])
{
	int status;
	
	VTable v;
	
	v.openFile(argv[1], argv[2], 0,  &status);
	cout << endl << "Output of Test Program starts here *****************" << endl;
	cout << "Status of VTABLE openFile is  " << status << endl;

	if (status != SUCCESS)
	{
		cout << "Error : " << getVOTableErrorMessage(status);
		cout << "VTABLE could not be opened properly." << endl;
		cout << "Exiting now." << endl;
		return ;
	}
	printVTable(v);
	 
	v.closeFile (&status);

}

/*
* This function tests the Resource by printing values in it.
*/
void testResource(const char*	argv[])
{
	int status;
	
	Resource r;
	
	r.openFile(argv[1], argv[2], 0,  &status);
	//cout << endl << "Output of Test Program starts here *****************" << endl;
	cout << "Status of Resource openFile is  " << status << endl;

	if (status != SUCCESS)
	{
		cout << "Error : " << getVOTableErrorMessage(status);
		cout << "Resource could not be opened properly." << endl;
		cout << "Exiting now." << endl;
		return ;
	}
	
	printResource(r);

	r.closeFile (&status);
}

/*
* Print the contents of 'Resource'.
*/
void printResource(Resource &r)
{
	char * str = NULL;
	int status;

	r.getDescription(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Resource description is " << str << endl;
		delete[] str;
	}

	r.getID(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Resource ID is " << str << endl;
		delete[] str;
	}

	r.getName(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Resource name is " << str << endl;
		delete[] str;
	}

	r.getUtype(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Resource utype is " << str << endl;
		delete[] str;
	}

	resource_type t;

	r.getType(t, &status);
	if (status == SUCCESS)
	{
		cout << "Resource type is " << t << endl;
	}

	// test INFO
	
	int nInfos = 0;
	if (r.getNumOfInfos(nInfos, &status) == SUCCESS && nInfos > 0)
	{
		Info info;
		for (int i = 0; i < nInfos; i++)
		{
			if (r.getInfo (info,i,&status) == SUCCESS)
			{
				printInfoAttributes(info, i + 1);
			}
		}
	}
	

	

	// test Coosys
	int nCS = 0;
	if (r.getNumOfCoosys(nCS, &status) == SUCCESS && nCS > 0)
	{
		Coosys c;
		for (int i = 0; i < nCS; i++)
		{
			if (r.getCoosys (c,i,&status) == SUCCESS)
			{
				printCoosysAttributes(c, i + 1);
			}
		}
	}
	

	

	// test Params
	int nParams = 0;
	if (r.getNumOfParams(nParams, &status) == SUCCESS && nParams > 0)
	{
		Param p;
		for (int i = 0; i < nParams; i++)
		{
			if (r.getParam (p, i, &status) == SUCCESS)
			{
				printParamAttributes(p, i + 1);
			}
		}
	}
	
	
	int nLinks = 0;
	if (r.getNumOfLinks(nLinks, &status) == SUCCESS && nLinks > 0)
	{
		Link l;
		for (int i = 0; i < nLinks; i++)
		{
			if (r.getLink (l,i,&status) == SUCCESS)
			{
				printLinkAttributes(l, i + 1);

			}
		}
	}

	// test VTables
	int nTables = 0;
	if (r.getNumOfTables(nTables, &status) == SUCCESS && nTables > 0)
	{
		cout << "Number of 'TABLE' elements inside are " << nTables << endl;
		VTable v;
		for (int i = 0; i < nTables; i++)
		{
			if (r.getTable (v, i, &status) == SUCCESS)
			{
				cout << "VTable " << (i+1) << endl;
				printVTable(v);
			}
		}
	}

	// test Resource
	int nRes = 0;
	if (r.getNumOfResources(nRes, &status) == SUCCESS && nRes > 0)
	{
		cout << endl<< "Number of 'RESOURCE' elements inside are " << nRes << endl;
		Resource res;
		for (int i = 0; i < nRes; i++)
		{
			if (r.getResource (res,i,&status) == SUCCESS)
			{
				cout << "Printing RESOURCE " << (i+1) << endl;
				printResource(res);
				cout << "Completed printing RESOURCE " << (i+1) << endl;

			}
		}
	}

	cout << endl << "Completed printing 'RESOURCE'." << endl << endl;
	return;

}


/*
* Print the contents of 'VTable'.
*/
void printVTable(VTable &v)
{
	char * str = NULL;
	int status;

	v.getName(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "VTable name is " << str << endl;
		delete[] str;
	}

	v.getID(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "VTable ID is " << str << endl;
		delete[] str;
	}

	v.getRef(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "VTable ref is " << str << endl;
		delete[] str;
	}

	v.getUtype(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "VTable utype is " << str << endl;
		delete[] str;
	}

	TableMetaData tmd;
	TableData td;
	BinaryData bd;
	FitsData fd;

	if (v.getMetaData(tmd, &status) == SUCCESS)
	{
		tmd.getDescription(str, &status);
		if (status == SUCCESS && str != NULL)
		{
			cout << "VTable description is " << str << endl;
			delete[] str;
		}

		// print 'Link'
		int nLinks = 0;
		if (tmd.getNumOfLinks(nLinks, &status) == SUCCESS && nLinks > 0)
		{
			for (int i = 0; i < nLinks; i++)
			{
				Link l;
				if (tmd.getLink(l, i, &status) == SUCCESS) 
				{
					printLinkAttributes(l, i+1);
				}
			}

		}

		int numOfCols = 0;
		int numOfLinks = 0;
		int numOfParams = 0;
		int numOfGroups = 0;

		tmd.getNumOfColumns(numOfCols, &status);
		tmd.getNumOfLinks(numOfLinks, &status);
		tmd.getNumOfParams(numOfParams, &status);
		tmd.getNumOfGroups(numOfGroups, &status);

		cout << "Number of fields is " << numOfCols << endl;
		cout << "Number of params is " << numOfParams << endl;
		cout << "Number of groups is " << numOfGroups << endl;
		cout << "Number of links is " << numOfLinks << endl;

		if (0 == numOfCols)
		{
			cout << "Number of columns is zero, therefore exiting." << endl;
		}

		vector <Field> fieldList;

		for (int i = 0; i < numOfCols; i++)
		{
			Field f;
		
			tmd.getField(f, i, &status);

			printFieldAttributes(f, i + 1);
			fieldList.push_back (f);

		} // end of 'for'


		for (int i = 0; i < numOfParams; i++)
		{
			Param p;
		
			tmd.getParam(p, i, &status);

			printParamAttributes(p, i + 1);	

		} // end of 'for'

		
		for (int i = 0; i < numOfGroups; i++)
		{
			Group g;
		
			tmd.getGroup(g, i, &status);

			printGroup(g, i + 1);

		} // end of 'for'


		if (v.getData(td, &status) == SUCCESS)
		{
			printTableData(td, fieldList);
		
		} // end of 'if'

		if (v.getBinaryData(bd, &status) == SUCCESS)
		{
			printBinaryData(bd);
		
		} // end of 'if'

		if (v.getFitsData(fd, &status) == SUCCESS)
		{
			printFitsData(fd);
		
		} // end of 'if'
	}
	return;

}


/*
* Print the contents of 'Group'.
*/
void printGroup(Group &g, int groupNum)
{
	char * str = NULL;
	int status;

	g.getDescription(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " description is " << str << endl;
		delete[] str;
	}

	g.getID(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " ID is " << str << endl;
		delete[] str;
	}

	g.getName(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " name is " << str << endl;
		delete[] str;
	}

	g.getUtype(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " utype is " << str << endl;
		delete[] str;
	}

	g.getRef(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " ref is " << str << endl;
		delete[] str;
	}

	g.getUcd(str, &status);
	if (status == SUCCESS && str != NULL)
	{
		cout << "Group " << groupNum << " ucd is " << str << endl;
		delete[] str;
	}

	
	// test Params
	int nParams = 0;
	if (g.getNumOfParams(nParams, &status) == SUCCESS && nParams > 0)
	{
		Param p;
		for (int i = 0; i < nParams; i++)
		{
			if (g.getParam (p, i, &status) == SUCCESS)
			{
				printParamAttributes(p, i + 1);
			}
		}
	}


	// test ParamRefs
	int nParamRefs = 0;
	if (g.getNumOfParamRefs(nParamRefs, &status) == SUCCESS && nParamRefs > 0)
	{
		ParamRef p;
		for (int i = 0; i < nParamRefs; i++)
		{
			if (g.getParamRef(p, i, &status) == SUCCESS)
			{
				printParamRefAttributes(p, i + 1);
			}
		}
	}

	// test FieldRefs
	int nFieldRefs = 0;
	if (g.getNumOfFieldRefs(nFieldRefs, &status) == SUCCESS && nFieldRefs > 0)
	{
		FieldRef f;
		for (int i = 0; i < nFieldRefs; i++)
		{
			if (g.getFieldRef(f, i, &status) == SUCCESS)
			{
				printFieldRefAttributes(f, i + 1);
			}
		}
	}
	
	
	
	// test Group
	int nGroups = 0;
	if (g.getNumOfGroups(nGroups, &status) == SUCCESS && nGroups > 0)
	{
		cout << endl<< "Number of 'Group' elements inside are " << nGroups << endl;
		Group grp;
		for (int i = 0; i < nGroups; i++)
		{
			if (g.getGroup (grp,i,&status) == SUCCESS)
			{
				printGroup(grp, i+1);
			}
		}
	}
	
	return;

}


/*
* Print the contents of 'Info'.
*/
void printInfoAttributes(Info &i, int infoNum)
{
	char * str = NULL;
	int status = 0;

	if (i.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Info " << infoNum << " ID is = " << str << endl;
		delete[] str;
	}

	if (i.getValue(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Info " << infoNum << " value is = " << str << endl;
		delete[] str;
	}

	if (i.getName(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Info " << infoNum << " name is = " << str << endl;
		delete[] str;
	}

	if (i.getPCData(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Info " << infoNum << " pcdata is = " << str << endl;
		delete[] str;
	}

}


/*
* Print the contents of 'Coosys'.
*/
void printCoosysAttributes(Coosys &c, int cNum)
{
	char * str = NULL;
	int status = 0;

	if (c.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Coosys " << cNum << " ID is = " << str << endl;
		delete[] str;
	}

	if (c.getEquinox(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Coosys " << cNum << " equinox is = " << str << endl;
		delete[] str;
	}

	if (c.getEpoch(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Coosys " << cNum << " epoch is = " << str << endl;
		delete[] str;
	}

	if (c.getPCData(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Coosys " << cNum << " pcdata is = " << str << endl;
		delete[] str;
	}

	coosys_system cs;
	if (c.getSystem(cs, &status) == SUCCESS)
	{
		cout << "Coosys " << cNum << " system is = " << cs << endl;
	}
	return;


}


/*
* Print the contents of 'Param'.
*/
void printParamAttributes(Param &f, int fieldNum)
{
	char * str = NULL;
	field_datatype dt;
	int status = 0;
	
	
	if (f.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " ID is = " << str << endl;
		delete[] str;
	}

	if (f.getName(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " name is = " << str << endl;
		delete[] str;
	}

	if (f.getUCD(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " UCD  is = " << str << endl;
		delete[] str;
	}

	if (f.getUtype(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " Utype  is = " << str << endl;
		delete[] str;
	}

	if (f.getDatatype(dt, &status) == SUCCESS)
	{
		cout << "Param " << fieldNum << " datatype is = " << dt << endl;

	}

	if (f.getArraySize(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " arraysize is = " << str << endl;
		delete[] str;
	}

	if (f.getRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " reference is = " << str << endl;
		delete[] str;
	}

	if (f.getUnit(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " unit is = " << str << endl;
		delete[] str;
	}

	if (f.getPrecision(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " precision is = " << str << endl;
		delete[] str;
	}

	int width;

	if (f.getWidth(width, &status) == SUCCESS)
	{
		cout << "Param " << fieldNum << " width is = " << width << endl;
	}

	if (f.getDescription (str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " description is = " << str << endl;
		delete[] str;
	}

	if (f.getValue (str, &status) == SUCCESS && str != NULL)
	{
		cout << "Param " << fieldNum << " value is = " << str << endl;
		delete[] str;
	}

	bool b;
	if (f.isVariableType(b, &status) == SUCCESS && b == true)
	{
		cout << "Param " << fieldNum << " is of variable length." << endl;
	}

	int nLinks = 0;
	if (f.getNumOfLinks(nLinks, &status) == SUCCESS && nLinks > 0)
	{
		Link l;
		for (int i = 0; i < nLinks; i++)
		{
			if (f.getLink (l,i,&status) == SUCCESS)
			{
				printLinkAttributes(l, i + 1);

			}
		}
	}
	int numOfValues = 0;


	if (f.getNumOfValues(numOfValues, &status) == SUCCESS && numOfValues > 0)
	{
		for (int i = 0; i < numOfValues; i++)
		{
			Values v;
			if (f.getValues (v, i, &status) == SUCCESS)
			{
				printValue(v, i + 1);
			}
		}
	}

	return;

}


/*
* Print the contents of 'Field'.
*/
void printFieldAttributes(Field &f, int fieldNum)
{
	char * str = NULL;
	field_datatype dt;
	int status = 0;
	
	
	if (f.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " ID is = " << str << endl;
		delete[] str;
	}

	if (f.getName(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " name is = " << str << endl;
		delete[] str;
	}

	if (f.getUCD(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " UCD  is = " << str << endl;
		delete[] str;
	}

	if (f.getUtype(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " Utype  is = " << str << endl;
		delete[] str;
	}

	if (f.getDatatype(dt, &status) == SUCCESS)
	{
		cout << "Field " << fieldNum << " datatype is = " << dt << endl;

	}

	field_type t;
	if (f.getType(t, &status) == SUCCESS)
	{
		cout << "Field " << fieldNum << " type is = " << t << endl;

	}

	if (f.getArraySize(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " arraysize is = " << str << endl;
		delete[] str;
	}

	if (f.getRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " reference is = " << str << endl;
		delete[] str;
	}

	if (f.getUnit(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " unit is = " << str << endl;
		delete[] str;
	}

	if (f.getPrecision(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " precision is = " << str << endl;
		delete[] str;
	}

	int width;

	if (f.getWidth(width, &status) == SUCCESS)
	{
		cout << "Field " << fieldNum << " width is = " << width << endl;
	}

	if (f.getDescription (str, &status) == SUCCESS && str != NULL)
	{
		cout << "Field " << fieldNum << " description is = " << str << endl;
		delete[] str;
	}

	bool b;
	if (f.isVariableType(b, &status) == SUCCESS && b == true)
	{
		cout << "Field " << fieldNum << " is of variable length." << endl;
	}

	int nLinks = 0;
	if (f.getNumOfLinks(nLinks, &status) == SUCCESS && nLinks > 0)
	{
		Link l;
		for (int i = 0; i < nLinks; i++)
		{
			if (f.getLink (l,i,&status) == SUCCESS)
			{
				printLinkAttributes(l, i + 1);

			}
		}
	}
	int numOfValues = 0;


	if (f.getNumOfValues(numOfValues, &status) == SUCCESS && numOfValues > 0)
	{
		for (int i = 0; i < numOfValues; i++)
		{
			Values v;
			if (f.getValues (v, i, &status) == SUCCESS)
			{
				printValue(v, i + 1);
			}
		}
	}

}


/*
* Print the contents of 'FieldRef'.
*/
void printFieldRefAttributes(FieldRef &f, int fieldNum)
{
	char * str = NULL;
	int status = 0;
	
	
	if (f.getRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "FieldRef " << fieldNum << " reference is = " << str << endl;
		delete[] str;
	}
}


/*
* Print the contents of 'ParamRef'.
*/
void printParamRefAttributes(ParamRef &p, int paramNum)
{
	char * str = NULL;
	int status = 0;
	
	
	if (p.getRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "ParamRef " << paramNum << " reference is = " << str << endl;
		delete[] str;
	}
}


/*
* Print the contents of 'Value'.
*/
void printValue(Values &v, int valueNum)
{

	char * str = NULL;
	int status = 0;

	if (v.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Value " << valueNum << " ID is = " << str << endl;
		delete[] str;
	}

	if (v.getNull(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Value " << valueNum << " null is = " << str << endl;
		delete[] str;
	}

	if (v.getRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Value " << valueNum << " Ref is = " << str << endl;
		delete[] str;
	}

	values_type t;
	if (v.getType(t, &status) == SUCCESS)
	{
		cout << "Value " << valueNum << " type is = " << t << endl;
	}

	bool b;
	if (v.isValid(b, &status) == SUCCESS)
	{
		cout << "Value " << valueNum << " invalid is = " << b << endl;
	}

	Range *r;
	
	if (v.getMinimun(r, &status) == SUCCESS && r != NULL)
	{
		printRange(*r);
		delete r;
	}

	if (v.getMaximum(r, &status) == SUCCESS && r != NULL)
	{
		printRange(*r);
		delete r;
	}

	Option o;
	int numOfOptions = 0;
	if (v.getNumOfOptions(numOfOptions, &status) == SUCCESS)
	{
		for (int i = 0; i < numOfOptions; i ++)
		{

			if (v.getOption(o, i, &status) == SUCCESS)
			{
				printOption(o, i + 1);
			}
		}
	}

	return;

}

/*
* Print the contents of 'Range'.
*/
void printRange(Range &r)
{
	char * str = NULL;
	int status = 0;

	if (r.getValue(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Range value is = " << str << endl;
		delete[] str;

	}

	if (r.getPCData(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Range pcdata is = " << str << endl;
		delete[] str;

	}

	bool b;

	if (r.isInclusive(b, &status) == SUCCESS)
	{
		cout << "Range inclusive is = " << b << endl;

	}

}


/*
* Print the contents of 'Option'.
*/
void printOption(Option &o, int optionNum)
{
	char *str;
	int status = 0;

	if(o.getName (str, &status) == SUCCESS && str != NULL)
	{
		cout << "Option " << optionNum << " name is = " << str << endl;
		delete[] str;
	}

	if(o.getValue (str, &status) == SUCCESS && str != NULL)
	{
		cout << "Option " << optionNum << " value is = " << str << endl;
		delete[] str;
	}

	int numOfOptions = 0;

	
	// get Option and print
	if(o.getNumOfOptions(numOfOptions, &status) == SUCCESS && numOfOptions > 0)
	{
		cout << "Options inside this 'OPTION' are " << endl;
		for(int i = 0; i < numOfOptions; i++)
		{
			Option option;
			if (o.getOption (option, i , &status) == SUCCESS)
			{
				printOption(option, i+1);
			}
		}
		cout << "Completed printing options inside 'OPTION' " << optionNum << endl;
	}
}

/*
* Print the contents of 'TableData'.
*/
void printTableData(TableData &td, vector<Field> &fieldList)
{
	int numOfRows = 0;
	int status = 0;
	char * str = NULL;

	if (td.getNumOfRows(numOfRows, &status) == SUCCESS)
	{
		cout << "Number of rows in this table data are " << numOfRows << endl;
	}
	field_datatype dt;

	// get each row
	for (int rowIndex = 0; rowIndex < numOfRows; rowIndex++)
	{
		Row r;
		if (td.getRow(r, rowIndex, &status) == SUCCESS)
		{
			// get each column of row and print it.
			
			Column c;
			int numOfElements = 0;
			
			for (int colIndex = 0; colIndex < fieldList.size (); colIndex++)
			{
				Field field = fieldList[colIndex];
				numOfElements = 0;
				if (field.getDatatype (dt, &status) == SUCCESS)
				{
					if (r.getColumn (c, colIndex, &status) == SUCCESS)
					{
						cout << endl << endl << "*** Row " << rowIndex + 1 << " Col " 
							<< (colIndex + 1) << " ***";
						if (c.getRef (str, &status) == SUCCESS && str != NULL)
						{
							cout << "Column ref is " << str << endl;
							delete[] str;
						}
						try {
						switch (dt) {
						case IntType :
							{
								int *intArray;

								if (c.getIntArray(intArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "type is int" << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << intArray[i] << " ";
									}
									
									delete[] intArray;
								}
								break;
							}

						case UnicodeCharType :
							{
								unsigned short *unicodeArray;
								if (c.getUnicodeArray (unicodeArray, numOfElements, &status) == SUCCESS)
								{
									
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Unicode data in decimal is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << unicodeArray[i] << " ";
										 
									}

									cout << endl << endl;
								

									// for printing unicodes in hex
									/*
									for (int j = 0; j < numOfElements; j ++)
									{
										for (int i=2*sizeof(short) - 1; i>=0; i--) 
										{
											 cout << "0123456789ABCDEF"[((unicodeArray[j] >> i*4) & 0xF)] ;
										}
										cout << " ";						
      									 
									}
									cout << endl << endl;
									*/
									
									
									delete[] unicodeArray;
								}
							}

							case UnsignedByteType :
							{
								unsigned char *charArray;
								if (c.getByteArray(charArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << (int)charArray[i] << " ";
									}
									
									delete[] charArray;
								}
								break;
							}

							case BitType :
							{
								char *cArray;
								if (c.getBitArray(cArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									// numOfBytes will contain the no of bytes which will hold the bits.
									int numOfBytes;

									if((numOfElements % 8) == 0)
									{
										numOfBytes = (numOfElements/8);
									}
									else 
									{
										numOfBytes = (numOfElements /8) + 1;
									}
									cout << "Data is " ;
									for (int i = 0; i < numOfBytes; i++)
									{
										if(numOfElements >= 8)
										{
											printBits(cArray[i], 8);
										}
										else
										{
											printBits(cArray[i], numOfElements);
										}

										numOfElements -= 8;
										// cout << cArray[i] << " ";
									}
									
									delete[] cArray;
								}
								break;
							}

						case LongType :
							{
								long *longArray;
								if (c.getLongArray(longArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << longArray[i] << " ";
									}
									
									delete[] longArray;
								}
								break;
							}

						case ShortType :
						{
							short *shortArray;
							if (c.getShortArray(shortArray, numOfElements, &status) == SUCCESS)
							{
								cout << endl << "Number of elements is " << numOfElements << endl;
								cout << "Data is " ;
								for (int i = 0; i < numOfElements; i ++)
								{
									 cout << shortArray[i] << " ";
								}
								delete[] shortArray;
							}
							break;
						}
						case FloatType :
							{
								float *floatArray;
								if (c.getFloatArray(floatArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << floatArray[i] << " ";
									}
									delete[] floatArray;
								}
								break;
							}

						case FloatComplexType :
							{
								float *floatArray;
								if (c.getFloatComplexArray(floatArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << floatArray[i] << " ";
									}
									delete[] floatArray;
								}
								break;
							}

						case DoubleType :
							{
								double *doubleArray;
								if (c.getDoubleArray(doubleArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << doubleArray[i] << " ";
									}
									delete[] doubleArray;
								}
								break;
							}

						case DoubleComplexType :
							{
								double *doubleArray;
								if (c.getDoubleComplexArray(doubleArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << doubleArray[i] << " ";
									}
									delete[] doubleArray;
								}
								break;
							}

						case BooleanType :
							{
								Bool *boolArray;
								if (c.getLogicalArray(boolArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is " ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << boolArray[i] << " ";
									}
									delete[] boolArray;
								}
								break;
							}

						case CharType :
							{
								char *charArray;
								
								if (c.getCharArray(charArray, numOfElements, &status) == SUCCESS)
								{
									cout << endl << "Number of elements is " << numOfElements << endl;
									cout << "Data is =>" ;
									for (int i = 0; i < numOfElements; i ++)
									{
										 cout << charArray[i] << " ";
									}
									cout << "<=" << endl;
									delete[] charArray;
								}
								
								break;
							}
						default : { cout << "Unknown Datatype !" << endl; break; }
						} // switch

						}
						catch(MissingValueException &m)
						{
							cout << m.getMessage();
						}
						catch(DatatypeMismatchException &ide)
						{
							cout << ide.getMessage();
						}


						} // if
				}

			} // end of 'for'

		}
	}


}


/*
* Print the contents of 'Binary'.
*/
void printBinaryData(BinaryData &bd)
{
	int status = 0;
	Stream s;
	if (bd.getStream(s, &status) == SUCCESS)
	{
		printBinaryStream(s);
	} // end of 'if'
}


void printBinaryStream(Stream &s)
{
	char * str = NULL;
	int status = 0;

	cout << endl << "Stream attributes :" << endl;

	printStreamAttributes(s);

	if (s.getData(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Data content --  " << endl << str << endl;
		delete[] str;
	}

}


void printStreamAttributes(Stream &s)
{
	char * str = NULL;
	int status = 0;

	if (s.getType(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Type = " << str << endl;
		delete[] str;
	}

	if (s.getHref(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Href = " << str << endl;
		delete[] str;
	}

	if (s.getEncoding(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Encoding = " << str << endl;
		delete[] str;
	}

	if (s.getActuate(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Actuate = " << str << endl;
		delete[] str;
	}

	if (s.getExpires(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Expires = " << str << endl;
		delete[] str;
	}

	if (s.getRights(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Rights = " << str << endl;
		delete[] str;
	}

	
	return;

}

/*
* Print the contents of 'Binary'.
*/
void printFitsData(FitsData &fd)
{
	char * str = NULL;
	int status = 0;

	Stream s;
	if (fd.getStream(s, &status) == SUCCESS)
	{
		printStreamAttributes(s);
	
	} // end of 'if'

	if (fd.getExtnum(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Rights = " << str << endl;
		delete[] str;
	}

}

/*
* Print the contents of 'Link'.
*/
void printLinkAttributes(Link &l, int linkNum)
{
	char * str = NULL;
	int status = 0;

	if (l.getID(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " ID is " << str << endl;
		delete[] str;
	}

	if (l.getHRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " HRef is " << str << endl;
		delete[] str;
	}

	if (l.getTitle(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " title is " << str << endl;
		delete[] str;
	}

	if (l.getGRef(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " GRef is " << str << endl;
		delete[] str;
	}

	if (l.getAction(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " action is " << str << endl;
		delete[] str;
	}

	if (l.getPCData(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " pcdata is " << str << endl;
		delete[] str;
	}

	if (l.getContentType(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " content type is " << str << endl;
		delete[] str;
	}

	if (l.getValue(str, &status) == SUCCESS && str != NULL)
	{
		cout << "Link " << linkNum << " value is " << str << endl;
		delete[] str;
	}

	content_role c;
	if (l.getContentRole(c, &status) == SUCCESS)
	{
		cout << "Link " << linkNum << " content role is " << c << endl;
		
	}


}


/*
* Print bits.
*/
void printBits(char c, int numOfBits)
{
	const unsigned char mask = (char) 0x80;
	unsigned int firstbit = 0;

	for (int i =0; i < numOfBits; i++)
	{
		firstbit = c & mask;
		if (firstbit == 0)
		{
			cout << "0 ";
		} 
		else 
		{
			cout << "1 ";
		}
	
		c = c << 1;
	}
	cout << "  ";

}

