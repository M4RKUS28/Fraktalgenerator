function Component()
{
	// default constructor
}

Component.prototype.createOperations = function()
{
	// call default implementation to actually install Fraktalgenerator.exe
	component.createOperations();
	
	if(systemInfo.productType === "windows") {
		component.addOperation("CreateShortcut", "@TargetDir@/Version/FrakGen3/Desktop/Windows/Fraktalgenerator.exe",
							"@StartMenuDir@/Fraktalgenerator3.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");
		component.addOperation("CreateShortcut", "@TargetDir@/Version/FrakGen3/Desktop/Windows/Fraktalgenerator.exe",
							"@HomeDir@/Desktop/Fraktalgenerator3.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");					
	
	}


}