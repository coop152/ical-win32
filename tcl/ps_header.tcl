proc ps_header {} { set val {}
  set val "$val%!PS-Adobe-2.0\n"
  set val "$val%%Creator: ical\n"
  set val "$val%%Title: Calendar listing\n"
  set val "$val%%Pages: 1\n"
  set val "$val%%PageOrder: Ascend\n"
  set val "$val%%DocumentFonts: Times-Roman\n"
  set val "$val%%EndComments\n"
  set val "$val\n"
  set val "$val% Simple defs\n"
  set val "$val/inch { 72 mul } def\n"
  set val "$val/mm { 72 mul 25.4 div } def\n"
  set val "$val\n"
  set val "$val% User-defined parameters (with defaults)\n"
  set val "$val/FontName       /Times-Roman    def\n"
  set val "$val/RealWidth      8.5 inch        def\n"
  set val "$val/RealHeight     11 inch         def\n"
  set val "$val/LandScape      true            def\n"
  set val "$val/GridWidth      4               def\n"
  set val "$val/GridHeight     2               def\n"
  set val "$val/BorderWidth    0.5 inch        def\n"
  set val "$val/BorderHeight   0.3 inch        def\n"
  set val "$val/HeaderHeight   0.5 inch        def\n"
  set val "$val/HeaderSep      0.2 inch        def\n"
  set val "$val/HeaderLeft     ()              def\n"
  set val "$val/HeaderCenter   ()              def\n"
  set val "$val/HeaderRight    ()              def\n"
  set val "$val/ColumnHeight   0               def\n"
  set val "$val/ColumnSep      0               def\n"
  set val "$val/MinLines       1               def\n"
  set val "$val/MaxLines       100             def\n"
  set val "$val/TitleFraction  0.16            def\n"
  set val "$val/FontHeight     16              def\n"
  set val "$val\n"
  set val "$val% User invoked commands\n"
  set val "$val/SetLandScape           { /LandScape true def }         def\n"
  set val "$val/SetPortrait            { /LandScape false def }        def\n"
  set val "$val/SetGridWidth           { /GridWidth exch def }         def\n"
  set val "$val/SetGridHeight          { /GridHeight exch def }        def\n"
  set val "$val/SetBorderWidth         { /BorderWidth exch def }       def\n"
  set val "$val/SetBorderHeight        { /BorderHeight exch def }      def\n"
  set val "$val/SetHeaderHeight        { /HeaderHeight exch def }      def\n"
  set val "$val/SetHeaderLeft          { /HeaderLeft exch def }        def\n"
  set val "$val/SetHeaderCenter        { /HeaderCenter exch def }      def\n"
  set val "$val/SetHeaderRight         { /HeaderRight exch def }       def\n"
  set val "$val/SetHeaderSep           { /HeaderSep exch def }         def\n"
  set val "$val/SetTitleFraction       { /TitleFraction exch def }     def\n"
  set val "$val/SetMinLines            { /MinLines exch def }          def\n"
  set val "$val/SetMaxLines            { /MaxLines exch def }          def\n"
  set val "$val/SetColumnHeight        { /ColumnHeight exch def }      def\n"
  set val "$val/SetColumnSep           { /ColumnSep exch def } def\n"
  set val "$val/SetFontHeight          { /FontHeight exch def } def\n"
  set val "$val/SetA4Paper             { /RealWidth 210 mm def /RealHeight 297 mm def} def\n"
  set val "$val/SetUSLetter            { /RealWidth 8.5 inch def /RealHeight 11 inch def } def\n"
  set val "$val\n"
  set val "$val% Define /ISOLatin1Encoding only if it's not already there.\n"
  set val "$val/ISOLatin1Encoding where { pop save true }{ false } ifelse\n"
  set val "$val    /ISOLatin1Encoding \[\n"
  set val "$val        StandardEncoding 0 45 getinterval aload pop\n"
  set val "$val        /minus\n"
  set val "$val        StandardEncoding 46 98 getinterval aload pop\n"
  set val "$val        /dotlessi /grave /acute /circumflex /tilde /macron /breve /dotaccent\n"
  set val "$val        /dieresis /.notdef /ring /cedilla /.notdef /hungarumlaut /ogonek\n"
  set val "$val        /caron /space /exclamdown /cent /sterling /currency /yen /brokenbar\n"
  set val "$val        /section /dieresis /copyright /ordfeminine /guillemotleft /logicalnot\n"
  set val "$val        /hyphen /registered /macron /degree /plusminus /twosuperior\n"
  set val "$val        /threesuperior /acute /mu /paragraph /periodcentered /cedilla\n"
  set val "$val        /onesuperior /ordmasculine /guillemotright /onequarter /onehalf\n"
  set val "$val        /threequarters /questiondown /Agrave /Aacute /Acircumflex /Atilde\n"
  set val "$val        /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex /Edieresis\n"
  set val "$val        /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute\n"
  set val "$val        /Ocircumflex /Otilde /Odieresis /multiply /Oslash /Ugrave /Uacute\n"
  set val "$val        /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute\n"
  set val "$val        /acircumflex /atilde /adieresis /aring /ae /ccedilla /egrave /eacute\n"
  set val "$val        /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth\n"
  set val "$val        /ntilde /ograve /oacute /ocircumflex /otilde /odieresis /divide /oslash\n"
  set val "$val        /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis\n"
  set val "$val    ] def\n"
  set val "$val{ restore } if\n"
  set val "$val\n"
  set val "$val/reencodeISO { %def\n"
  set val "$val    findfont dup length dict\n"
  set val "$val    begin\n"
  set val "$val        { 1 index /FID ne { def }{ pop pop } ifelse } forall\n"
  set val "$val        /Encoding ISOLatin1Encoding def\n"
  set val "$val        currentdict\n"
  set val "$val    end\n"
  set val "$val    definefont pop\n"
  set val "$val} bind def\n"
  set val "$val\n"
  set val "$val/isoFont FontName reencodeISO\n"
  set val "$val/FontName /isoFont def\n"
  set val "$val\n"
  set val "$val% Compute layout\n"
  set val "$val/ComputeLayout {\n"
  set val "$val    % Adjust for landscape\n"
  set val "$val    LandScape {\n"
  set val "$val        /PageWidth RealHeight def\n"
  set val "$val        /PageHeight RealWidth def\n"
  set val "$val        RealWidth 0 translate\n"
  set val "$val        90 rotate\n"
  set val "$val    }\n"
  set val "$val    {\n"
  set val "$val        /PageWidth RealWidth def\n"
  set val "$val        /PageHeight RealHeight def\n"
  set val "$val    } ifelse\n"
  set val "$val\n"
  set val "$val    /DayWidth PageWidth BorderWidth 2 mul sub GridWidth div def\n"
  set val "$val    /DayHeight PageHeight HeaderHeight sub HeaderSep sub\n"
  set val "$val               ColumnHeight sub ColumnSep sub\n"
  set val "$val               BorderHeight 2 mul sub GridHeight div def\n"
  set val "$val    /DayTitleHeight DayHeight TitleFraction mul def\n"
  set val "$val\n"
  set val "$val    % Layout the header\n"
  set val "$val    newpath\n"
  set val "$val        /HeaderY PageHeight BorderHeight sub HeaderHeight sub def\n"
  set val "$val        FontName findfont HeaderHeight 0.8 mul scalefont setfont\n"
  set val "$val\n"
  set val "$val        % Place left string\n"
  set val "$val        BorderWidth HeaderY moveto\n"
  set val "$val        HeaderLeft show\n"
  set val "$val\n"
  set val "$val        % Place center string\n"
  set val "$val        PageWidth HeaderCenter stringwidth pop sub 2 div HeaderY moveto\n"
  set val "$val        HeaderCenter show\n"
  set val "$val\n"
  set val "$val        % Place right string\n"
  set val "$val        PageWidth BorderWidth sub HeaderRight stringwidth pop sub\n"
  set val "$val        HeaderY moveto\n"
  set val "$val        HeaderRight show\n"
  set val "$val   stroke\n"
  set val "$val\n"
  set val "$val   % Translate to grid origin\n"
  set val "$val   BorderWidth BorderHeight translate\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Draw the day boundary\n"
  set val "$val/DayBoundary {\n"
  set val "$val    0 0 moveto DayWidth DayHeight DrawBox\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Draw the title boundary.\n"
  set val "$val/DayTitleBoundary {\n"
  set val "$val    DayLeft () eq {\n"
  set val "$val        % Left is empty. Title is just right justified box\n"
  set val "$val\n"
  set val "$val        % Compute right string width\n"
  set val "$val        FontName findfont DayTitleHeight 0.8 mul scalefont setfont\n"
  set val "$val        DayRight stringwidth pop 1.4 mul\n"
  set val "$val        /DayRightWidth exch def\n"
  set val "$val\n"
  set val "$val        % Draw box\n"
  set val "$val        DayWidth DayRightWidth sub DayHeight DayTitleHeight sub moveto\n"
  set val "$val        DayRightWidth DayTitleHeight DrawBox\n"
  set val "$val    }\n"
  set val "$val    {\n"
  set val "$val        % Title extends for entire DayWidth\n"
  set val "$val        0 DayHeight DayTitleHeight sub moveto\n"
  set val "$val        DayWidth DayTitleHeight DrawBox\n"
  set val "$val    }\n"
  set val "$val    ifelse\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Fill in the titles\n"
  set val "$val/DayTitle {\n"
  set val "$val    FontName findfont DayTitleHeight 0.8 mul scalefont setfont\n"
  set val "$val\n"
  set val "$val    % Show DayLeft\n"
  set val "$val    0 DayHeight DayTitleHeight sub moveto\n"
  set val "$val    DayTitleHeight 0.2 mul dup rmoveto\n"
  set val "$val    DayLeft show\n"
  set val "$val\n"
  set val "$val    % Show DayRight\n"
  set val "$val    0 DayHeight DayTitleHeight sub moveto\n"
  set val "$val    DayWidth DayTitleHeight 0.2 mul sub DayTitleHeight 0.2 mul rmoveto\n"
  set val "$val    DayRight stringwidth pop neg 0 rmoveto\n"
  set val "$val    DayRight show\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Draw the box for a particular day at the current position\n"
  set val "$val% Input:\n"
  set val "$val%       left:   string\n"
  set val "$val%       right:  string\n"
  set val "$val%       list:   array\[array\[string]]\n"
  set val "$val\n"
  set val "$val/DayDraw {\n"
  set val "$val    /DayList  exch def\n"
  set val "$val    /DayRight exch def\n"
  set val "$val    /DayLeft  exch def\n"
  set val "$val\n"
  set val "$val    % Fill title box\n"
  set val "$val    currentpoint translate\n"
  set val "$val\n"
  set val "$val    gsave newpath DayTitleBoundary 0.9 setgray    fill   grestore \n"
  set val "$val    gsave newpath DayTitleBoundary 1 setlinewidth stroke grestore \n"
  set val "$val    gsave newpath DayTitle                        fill   grestore\n"
  set val "$val    gsave newpath DayBoundary      3 setlinewidth stroke grestore\n"
  set val "$val\n"
  set val "$val    % Set clipping for text\n"
  set val "$val    newpath DayTitleBoundary DayBoundary eoclip\n"
  set val "$val\n"
  set val "$val    % Get line width\n"
  set val "$val    /DayBorder 4 def\n"
  set val "$val    /LineWidth DayWidth DayBorder 4 mul sub def\n"
  set val "$val\n"
  set val "$val    % Set-up the font in the current height\n"
  set val "$val    /CurHeight FontHeight def\n"
  set val "$val    /LineHeight CurHeight 1.1 mul def\n"
  set val "$val    FontName findfont CurHeight scalefont setfont\n"
  set val "$val\n"
  set val "$val    % Compute available space\n"
  set val "$val    /Space DayHeight DayTitleHeight sub DayBorder 2 mul sub def\n"
  set val "$val    /ThisTitleHeight DayTitleHeight def\n"
  set val "$val\n"
  set val "$val    DayLeft () eq {\n"
  set val "$val      % The items can be allowed to overlap with the title if necessary\n"
  set val "$val      Space LineHeight div DayList LineWidth ComputeItemLines lt {\n"
  set val "$val        % Not enough space.  Set-up things for overlap\n"
  set val "$val        /ThisTitleHeight 0 def\n"
  set val "$val        /Space Space DayTitleHeight add def\n"
  set val "$val      } if\n"
  set val "$val    } if\n"
  set val "$val\n"
  set val "$val    % Scale down the font size to fit space\n"
  set val "$val    {\n"
  set val "$val      % Check to see if item fits within space\n"
  set val "$val      Space LineHeight div DayList LineWidth ComputeItemLines ge {\n"
  set val "$val        exit\n"
  set val "$val      } if\n"
  set val "$val\n"
  set val "$val      % Need to shrink the font height\n"
  set val "$val      /CurHeight CurHeight 1 sub def\n"
  set val "$val      /LineHeight CurHeight 1.1 mul def\n"
  set val "$val      FontName findfont CurHeight scalefont setfont\n"
  set val "$val    } loop\n"
  set val "$val\n"
  set val "$val    /DayPosition DayHeight ThisTitleHeight sub LineHeight 0.9 mul sub def\n"
  set val "$val\n"
  set val "$val    /str 100 string def\n"
  set val "$val    DayList {\n"
  set val "$val      % Display an item (list of strings).\n"
  set val "$val      {\n"
  set val "$val        newpath\n"
  set val "$val        LineWidth\n"
  set val "$val        {\n"
  set val "$val          DayBorder 2 mul DayPosition moveto show\n"
  set val "$val          /DayPosition DayPosition LineHeight sub def\n"
  set val "$val        }\n"
  set val "$val        BreakIntoLines\n"
  set val "$val        closepath fill\n"
  set val "$val      } forall\n"
  set val "$val\n"
  set val "$val      % Draw line at end\n"
  set val "$val      newpath\n"
  set val "$val      0 DayPosition LineHeight 0.8 mul add moveto\n"
  set val "$val      DayWidth 0 rlineto\n"
  set val "$val      closepath 0 setlinewidth stroke\n"
  set val "$val    } forall\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Draw box\n"
  set val "$val% Input:\n"
  set val "$val%       width height\n"
  set val "$val/DrawBox {\n"
  set val "$val    exch dup 0 rlineto\n"
  set val "$val    exch dup 0 exch rlineto\n"
  set val "$val    exch neg 0 rlineto\n"
  set val "$val    neg 0 exch rlineto\n"
  set val "$val    closepath\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Show day\n"
  set val "$val% Input:\n"
  set val "$val%       row\n"
  set val "$val%       column\n"
  set val "$val%       left text\n"
  set val "$val%       right text\n"
  set val "$val%       items\n"
  set val "$val/ShowDay {\n"
  set val "$val    5 3 roll\n"
  set val "$val    DayWidth mul exch GridHeight exch sub 1 sub DayHeight mul moveto\n"
  set val "$val\n"
  set val "$val    gsave DayDraw grestore\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% Show Column heading\n"
  set val "$val% Input:\n"
  set val "$val%       column\n"
  set val "$val%       ltext\n"
  set val "$val%       ctext\n"
  set val "$val%       rtext\n"
  set val "$val/ColumnHead {\n"
  set val "$val    /ColRight  exch def\n"
  set val "$val    /ColCenter exch def\n"
  set val "$val    /ColLeft   exch def\n"
  set val "$val    /ColNum    exch def\n"
  set val "$val\n"
  set val "$val    gsave\n"
  set val "$val        DayWidth ColNum mul GridHeight DayHeight mul ColumnSep add translate\n"
  set val "$val        FontName findfont ColumnHeight 0.8 mul scalefont setfont\n"
  set val "$val\n"
  set val "$val        % Show left\n"
  set val "$val        newpath 0 0 moveto ColLeft show stroke\n"
  set val "$val\n"
  set val "$val        % Show center\n"
  set val "$val        newpath DayWidth ColCenter stringwidth pop sub 2 div 0 moveto\n"
  set val "$val                ColCenter show stroke\n"
  set val "$val\n"
  set val "$val        % Show right\n"
  set val "$val        newpath DayWidth ColRight stringwidth pop sub 0 moveto\n"
  set val "$val                ColRight show stroke\n"
  set val "$val    grestore\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% usage         <list> <width> \"ComputeItemLines\" <height>\n"
  set val "$val% requires      <list> is an array of array of strings.\n"
  set val "$val% effects       Compute the # of lines occupied by <list> if it\n"
  set val "$val%               is broken into lines of the specified width.\n"
  set val "$val\n"
  set val "$val/ComputeItemLines {\n"
  set val "$val  100 dict begin\n"
  set val "$val    /linewidth exch def\n"
  set val "$val    /lines 0 def\n"
  set val "$val\n"
  set val "$val    {\n"
  set val "$val      {\n"
  set val "$val        linewidth {\n"
  set val "$val          pop /lines 1 lines add def\n"
  set val "$val        } BreakIntoLines\n"
  set val "$val      } forall\n"
  set val "$val    } forall\n"
  set val "$val\n"
  set val "$val    lines\n"
  set val "$val  end\n"
  set val "$val} def\n"
  set val "$val\n"
  set val "$val% usage         <string> <width> <proc> \"BreakIntoLines\" -\n"
  set val "$val% requires      <proc> takes a single string argument\n"
  set val "$val% effects       line-breaks the <string> based on <width>.\n"
  set val "$val%               Calls <proc> with the contents of each line.\n"
  set val "$val\n"
  set val "$val/wordbreak ( ) def\n"
  set val "$val/BreakIntoLines {\n"
  set val "$val  /breakproc exch def\n"
  set val "$val  /linelength exch def\n"
  set val "$val  /textstring exch def\n"
  set val "$val\n"
  set val "$val  /breakwidth wordbreak stringwidth pop def\n"
  set val "$val  /curwidth 0 def\n"
  set val "$val  /lastwordbreak 0 def\n"
  set val "$val        \n"
  set val "$val  /startchar 0 def\n"
  set val "$val  /restoftext textstring def\n"
  set val "$val        \n"
  set val "$val  { restoftext wordbreak search\n"
  set val "$val    { /nextword exch def pop\n"
  set val "$val      /restoftext exch def\n"
  set val "$val      /wordwidth nextword stringwidth pop def\n"
  set val "$val                \n"
  set val "$val      curwidth wordwidth add linelength gt\n"
  set val "$val      { textstring startchar\n"
  set val "$val        lastwordbreak startchar sub\n"
  set val "$val        getinterval breakproc\n"
  set val "$val        /startchar lastwordbreak def\n"
  set val "$val        /curwidth wordwidth breakwidth add def }\n"
  set val "$val      { /curwidth curwidth wordwidth add\n"
  set val "$val        breakwidth add def \n"
  set val "$val      } ifelse\n"
  set val "$val\n"
  set val "$val      /lastwordbreak lastwordbreak\n"
  set val "$val      nextword length add 1 add def\n"
  set val "$val    }\n"
  set val "$val    {pop exit}\n"
  set val "$val    ifelse\n"
  set val "$val  } loop\n"
  set val "$val  /lastchar textstring length def\n"
  set val "$val  textstring startchar lastchar startchar sub \n"
  set val "$val  getinterval breakproc\n"
  set val "$val} def\n"
  set val "$val\n"
  return $val\n}
