proc ps_header {} { set val {}
  set val "$val%!PS-Adobe-2.0
  set val "$val%%Creator: ical
  set val "$val%%Title: Calendar listing
  set val "$val%%Pages: 1
  set val "$val%%PageOrder: Ascend
  set val "$val%%DocumentFonts: Times-Roman
  set val "$val%%EndComments
  set val "$val
  set val "$val% Simple defs
  set val "$val/inch { 72 mul } def
  set val "$val/mm { 72 mul 25.4 div } def
  set val "$val
  set val "$val% User-defined parameters (with defaults)
  set val "$val/FontName       /Times-Roman    def
  set val "$val/RealWidth      8.5 inch        def
  set val "$val/RealHeight     11 inch         def
  set val "$val/LandScape      true            def
  set val "$val/GridWidth      4               def
  set val "$val/GridHeight     2               def
  set val "$val/BorderWidth    0.5 inch        def
  set val "$val/BorderHeight   0.3 inch        def
  set val "$val/HeaderHeight   0.5 inch        def
  set val "$val/HeaderSep      0.2 inch        def
  set val "$val/HeaderLeft     ()              def
  set val "$val/HeaderCenter   ()              def
  set val "$val/HeaderRight    ()              def
  set val "$val/ColumnHeight   0               def
  set val "$val/ColumnSep      0               def
  set val "$val/MinLines       1               def
  set val "$val/MaxLines       100             def
  set val "$val/TitleFraction  0.16            def
  set val "$val/FontHeight     16              def
  set val "$val
  set val "$val% User invoked commands
  set val "$val/SetLandScape           { /LandScape true def }         def
  set val "$val/SetPortrait            { /LandScape false def }        def
  set val "$val/SetGridWidth           { /GridWidth exch def }         def
  set val "$val/SetGridHeight          { /GridHeight exch def }        def
  set val "$val/SetBorderWidth         { /BorderWidth exch def }       def
  set val "$val/SetBorderHeight        { /BorderHeight exch def }      def
  set val "$val/SetHeaderHeight        { /HeaderHeight exch def }      def
  set val "$val/SetHeaderLeft          { /HeaderLeft exch def }        def
  set val "$val/SetHeaderCenter        { /HeaderCenter exch def }      def
  set val "$val/SetHeaderRight         { /HeaderRight exch def }       def
  set val "$val/SetHeaderSep           { /HeaderSep exch def }         def
  set val "$val/SetTitleFraction       { /TitleFraction exch def }     def
  set val "$val/SetMinLines            { /MinLines exch def }          def
  set val "$val/SetMaxLines            { /MaxLines exch def }          def
  set val "$val/SetColumnHeight        { /ColumnHeight exch def }      def
  set val "$val/SetColumnSep           { /ColumnSep exch def } def
  set val "$val/SetFontHeight          { /FontHeight exch def } def
  set val "$val/SetA4Paper             { /RealWidth 210 mm def /RealHeight 297 mm def} def
  set val "$val/SetUSLetter            { /RealWidth 8.5 inch def /RealHeight 11 inch def } def
  set val "$val
  set val "$val% Define /ISOLatin1Encoding only if it's not already there.
  set val "$val/ISOLatin1Encoding where { pop save true }{ false } ifelse
  set val "$val    /ISOLatin1Encoding \[
  set val "$val        StandardEncoding 0 45 getinterval aload pop
  set val "$val        /minus
  set val "$val        StandardEncoding 46 98 getinterval aload pop
  set val "$val        /dotlessi /grave /acute /circumflex /tilde /macron /breve /dotaccent
  set val "$val        /dieresis /.notdef /ring /cedilla /.notdef /hungarumlaut /ogonek
  set val "$val        /caron /space /exclamdown /cent /sterling /currency /yen /brokenbar
  set val "$val        /section /dieresis /copyright /ordfeminine /guillemotleft /logicalnot
  set val "$val        /hyphen /registered /macron /degree /plusminus /twosuperior
  set val "$val        /threesuperior /acute /mu /paragraph /periodcentered /cedilla
  set val "$val        /onesuperior /ordmasculine /guillemotright /onequarter /onehalf
  set val "$val        /threequarters /questiondown /Agrave /Aacute /Acircumflex /Atilde
  set val "$val        /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex /Edieresis
  set val "$val        /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute
  set val "$val        /Ocircumflex /Otilde /Odieresis /multiply /Oslash /Ugrave /Uacute
  set val "$val        /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute
  set val "$val        /acircumflex /atilde /adieresis /aring /ae /ccedilla /egrave /eacute
  set val "$val        /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth
  set val "$val        /ntilde /ograve /oacute /ocircumflex /otilde /odieresis /divide /oslash
  set val "$val        /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis
  set val "$val    ] def
  set val "$val{ restore } if
  set val "$val
  set val "$val/reencodeISO { %def
  set val "$val    findfont dup length dict
  set val "$val    begin
  set val "$val        { 1 index /FID ne { def }{ pop pop } ifelse } forall
  set val "$val        /Encoding ISOLatin1Encoding def
  set val "$val        currentdict
  set val "$val    end
  set val "$val    definefont pop
  set val "$val} bind def
  set val "$val
  set val "$val/isoFont FontName reencodeISO
  set val "$val/FontName /isoFont def
  set val "$val
  set val "$val% Compute layout
  set val "$val/ComputeLayout {
  set val "$val    % Adjust for landscape
  set val "$val    LandScape {
  set val "$val        /PageWidth RealHeight def
  set val "$val        /PageHeight RealWidth def
  set val "$val        RealWidth 0 translate
  set val "$val        90 rotate
  set val "$val    }
  set val "$val    {
  set val "$val        /PageWidth RealWidth def
  set val "$val        /PageHeight RealHeight def
  set val "$val    } ifelse
  set val "$val
  set val "$val    /DayWidth PageWidth BorderWidth 2 mul sub GridWidth div def
  set val "$val    /DayHeight PageHeight HeaderHeight sub HeaderSep sub
  set val "$val               ColumnHeight sub ColumnSep sub
  set val "$val               BorderHeight 2 mul sub GridHeight div def
  set val "$val    /DayTitleHeight DayHeight TitleFraction mul def
  set val "$val
  set val "$val    % Layout the header
  set val "$val    newpath
  set val "$val        /HeaderY PageHeight BorderHeight sub HeaderHeight sub def
  set val "$val        FontName findfont HeaderHeight 0.8 mul scalefont setfont
  set val "$val
  set val "$val        % Place left string
  set val "$val        BorderWidth HeaderY moveto
  set val "$val        HeaderLeft show
  set val "$val
  set val "$val        % Place center string
  set val "$val        PageWidth HeaderCenter stringwidth pop sub 2 div HeaderY moveto
  set val "$val        HeaderCenter show
  set val "$val
  set val "$val        % Place right string
  set val "$val        PageWidth BorderWidth sub HeaderRight stringwidth pop sub
  set val "$val        HeaderY moveto
  set val "$val        HeaderRight show
  set val "$val   stroke
  set val "$val
  set val "$val   % Translate to grid origin
  set val "$val   BorderWidth BorderHeight translate
  set val "$val} def
  set val "$val
  set val "$val% Draw the day boundary
  set val "$val/DayBoundary {
  set val "$val    0 0 moveto DayWidth DayHeight DrawBox
  set val "$val} def
  set val "$val
  set val "$val% Draw the title boundary.
  set val "$val/DayTitleBoundary {
  set val "$val    DayLeft () eq {
  set val "$val        % Left is empty. Title is just right justified box
  set val "$val
  set val "$val        % Compute right string width
  set val "$val        FontName findfont DayTitleHeight 0.8 mul scalefont setfont
  set val "$val        DayRight stringwidth pop 1.4 mul
  set val "$val        /DayRightWidth exch def
  set val "$val
  set val "$val        % Draw box
  set val "$val        DayWidth DayRightWidth sub DayHeight DayTitleHeight sub moveto
  set val "$val        DayRightWidth DayTitleHeight DrawBox
  set val "$val    }
  set val "$val    {
  set val "$val        % Title extends for entire DayWidth
  set val "$val        0 DayHeight DayTitleHeight sub moveto
  set val "$val        DayWidth DayTitleHeight DrawBox
  set val "$val    }
  set val "$val    ifelse
  set val "$val} def
  set val "$val
  set val "$val% Fill in the titles
  set val "$val/DayTitle {
  set val "$val    FontName findfont DayTitleHeight 0.8 mul scalefont setfont
  set val "$val
  set val "$val    % Show DayLeft
  set val "$val    0 DayHeight DayTitleHeight sub moveto
  set val "$val    DayTitleHeight 0.2 mul dup rmoveto
  set val "$val    DayLeft show
  set val "$val
  set val "$val    % Show DayRight
  set val "$val    0 DayHeight DayTitleHeight sub moveto
  set val "$val    DayWidth DayTitleHeight 0.2 mul sub DayTitleHeight 0.2 mul rmoveto
  set val "$val    DayRight stringwidth pop neg 0 rmoveto
  set val "$val    DayRight show
  set val "$val} def
  set val "$val
  set val "$val% Draw the box for a particular day at the current position
  set val "$val% Input:
  set val "$val%       left:   string
  set val "$val%       right:  string
  set val "$val%       list:   array\[array\[string]]
  set val "$val
  set val "$val/DayDraw {
  set val "$val    /DayList  exch def
  set val "$val    /DayRight exch def
  set val "$val    /DayLeft  exch def
  set val "$val
  set val "$val    % Fill title box
  set val "$val    currentpoint translate
  set val "$val
  set val "$val    gsave newpath DayTitleBoundary 0.9 setgray    fill   grestore 
  set val "$val    gsave newpath DayTitleBoundary 1 setlinewidth stroke grestore 
  set val "$val    gsave newpath DayTitle                        fill   grestore
  set val "$val    gsave newpath DayBoundary      3 setlinewidth stroke grestore
  set val "$val
  set val "$val    % Set clipping for text
  set val "$val    newpath DayTitleBoundary DayBoundary eoclip
  set val "$val
  set val "$val    % Get line width
  set val "$val    /DayBorder 4 def
  set val "$val    /LineWidth DayWidth DayBorder 4 mul sub def
  set val "$val
  set val "$val    % Set-up the font in the current height
  set val "$val    /CurHeight FontHeight def
  set val "$val    /LineHeight CurHeight 1.1 mul def
  set val "$val    FontName findfont CurHeight scalefont setfont
  set val "$val
  set val "$val    % Compute available space
  set val "$val    /Space DayHeight DayTitleHeight sub DayBorder 2 mul sub def
  set val "$val    /ThisTitleHeight DayTitleHeight def
  set val "$val
  set val "$val    DayLeft () eq {
  set val "$val      % The items can be allowed to overlap with the title if necessary
  set val "$val      Space LineHeight div DayList LineWidth ComputeItemLines lt {
  set val "$val        % Not enough space.  Set-up things for overlap
  set val "$val        /ThisTitleHeight 0 def
  set val "$val        /Space Space DayTitleHeight add def
  set val "$val      } if
  set val "$val    } if
  set val "$val
  set val "$val    % Scale down the font size to fit space
  set val "$val    {
  set val "$val      % Check to see if item fits within space
  set val "$val      Space LineHeight div DayList LineWidth ComputeItemLines ge {
  set val "$val        exit
  set val "$val      } if
  set val "$val
  set val "$val      % Need to shrink the font height
  set val "$val      /CurHeight CurHeight 1 sub def
  set val "$val      /LineHeight CurHeight 1.1 mul def
  set val "$val      FontName findfont CurHeight scalefont setfont
  set val "$val    } loop
  set val "$val
  set val "$val    /DayPosition DayHeight ThisTitleHeight sub LineHeight 0.9 mul sub def
  set val "$val
  set val "$val    /str 100 string def
  set val "$val    DayList {
  set val "$val      % Display an item (list of strings).
  set val "$val      {
  set val "$val        newpath
  set val "$val        LineWidth
  set val "$val        {
  set val "$val          DayBorder 2 mul DayPosition moveto show
  set val "$val          /DayPosition DayPosition LineHeight sub def
  set val "$val        }
  set val "$val        BreakIntoLines
  set val "$val        closepath fill
  set val "$val      } forall
  set val "$val
  set val "$val      % Draw line at end
  set val "$val      newpath
  set val "$val      0 DayPosition LineHeight 0.8 mul add moveto
  set val "$val      DayWidth 0 rlineto
  set val "$val      closepath 0 setlinewidth stroke
  set val "$val    } forall
  set val "$val} def
  set val "$val
  set val "$val% Draw box
  set val "$val% Input:
  set val "$val%       width height
  set val "$val/DrawBox {
  set val "$val    exch dup 0 rlineto
  set val "$val    exch dup 0 exch rlineto
  set val "$val    exch neg 0 rlineto
  set val "$val    neg 0 exch rlineto
  set val "$val    closepath
  set val "$val} def
  set val "$val
  set val "$val% Show day
  set val "$val% Input:
  set val "$val%       row
  set val "$val%       column
  set val "$val%       left text
  set val "$val%       right text
  set val "$val%       items
  set val "$val/ShowDay {
  set val "$val    5 3 roll
  set val "$val    DayWidth mul exch GridHeight exch sub 1 sub DayHeight mul moveto
  set val "$val
  set val "$val    gsave DayDraw grestore
  set val "$val} def
  set val "$val
  set val "$val% Show Column heading
  set val "$val% Input:
  set val "$val%       column
  set val "$val%       ltext
  set val "$val%       ctext
  set val "$val%       rtext
  set val "$val/ColumnHead {
  set val "$val    /ColRight  exch def
  set val "$val    /ColCenter exch def
  set val "$val    /ColLeft   exch def
  set val "$val    /ColNum    exch def
  set val "$val
  set val "$val    gsave
  set val "$val        DayWidth ColNum mul GridHeight DayHeight mul ColumnSep add translate
  set val "$val        FontName findfont ColumnHeight 0.8 mul scalefont setfont
  set val "$val
  set val "$val        % Show left
  set val "$val        newpath 0 0 moveto ColLeft show stroke
  set val "$val
  set val "$val        % Show center
  set val "$val        newpath DayWidth ColCenter stringwidth pop sub 2 div 0 moveto
  set val "$val                ColCenter show stroke
  set val "$val
  set val "$val        % Show right
  set val "$val        newpath DayWidth ColRight stringwidth pop sub 0 moveto
  set val "$val                ColRight show stroke
  set val "$val    grestore
  set val "$val} def
  set val "$val
  set val "$val% usage         <list> <width> \"ComputeItemLines\" <height>
  set val "$val% requires      <list> is an array of array of strings.
  set val "$val% effects       Compute the # of lines occupied by <list> if it
  set val "$val%               is broken into lines of the specified width.
  set val "$val
  set val "$val/ComputeItemLines {
  set val "$val  100 dict begin
  set val "$val    /linewidth exch def
  set val "$val    /lines 0 def
  set val "$val
  set val "$val    {
  set val "$val      {
  set val "$val        linewidth {
  set val "$val          pop /lines 1 lines add def
  set val "$val        } BreakIntoLines
  set val "$val      } forall
  set val "$val    } forall
  set val "$val
  set val "$val    lines
  set val "$val  end
  set val "$val} def
  set val "$val
  set val "$val% usage         <string> <width> <proc> \"BreakIntoLines\" -
  set val "$val% requires      <proc> takes a single string argument
  set val "$val% effects       line-breaks the <string> based on <width>.
  set val "$val%               Calls <proc> with the contents of each line.
  set val "$val
  set val "$val/wordbreak ( ) def
  set val "$val/BreakIntoLines {
  set val "$val  /breakproc exch def
  set val "$val  /linelength exch def
  set val "$val  /textstring exch def
  set val "$val
  set val "$val  /breakwidth wordbreak stringwidth pop def
  set val "$val  /curwidth 0 def
  set val "$val  /lastwordbreak 0 def
  set val "$val        
  set val "$val  /startchar 0 def
  set val "$val  /restoftext textstring def
  set val "$val        
  set val "$val  { restoftext wordbreak search
  set val "$val    { /nextword exch def pop
  set val "$val      /restoftext exch def
  set val "$val      /wordwidth nextword stringwidth pop def
  set val "$val                
  set val "$val      curwidth wordwidth add linelength gt
  set val "$val      { textstring startchar
  set val "$val        lastwordbreak startchar sub
  set val "$val        getinterval breakproc
  set val "$val        /startchar lastwordbreak def
  set val "$val        /curwidth wordwidth breakwidth add def }
  set val "$val      { /curwidth curwidth wordwidth add
  set val "$val        breakwidth add def 
  set val "$val      } ifelse
  set val "$val
  set val "$val      /lastwordbreak lastwordbreak
  set val "$val      nextword length add 1 add def
  set val "$val    }
  set val "$val    {pop exit}
  set val "$val    ifelse
  set val "$val  } loop
  set val "$val  /lastchar textstring length def
  set val "$val  textstring startchar lastchar startchar sub 
  set val "$val  getinterval breakproc
  set val "$val} def
  set val "$val
  return $val\n}