function Component()
{
	// default constructor
}

Component.prototype.createOperations = function()
{
	// call default implementation to actually install Fraktalgenerator.exe
	component.createOperations();
	
	if(systemInfo.productType === "windows") {
		component.addOperation("CreateShortcut", "@TargetDir@/Version/FrakGen2/Fraktalgenerator.exe",
							"@StartMenuDir@/Fraktalgenerator2.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");
		component.addOperation("CreateShortcut", "@TargetDir@/Version/FrakGen2/Fraktalgenerator.exe",
							"@HomeDir@/Desktop/Fraktalgenerator2.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");					
	
	}


}