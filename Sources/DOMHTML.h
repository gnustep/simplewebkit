/* simplewebkit
   DOMHTML.h

   Copyright (C) 2007 Free Software Foundation, Inc.

   Author: Dr. H. Nikolaus Schaller

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; see the file COPYING.LIB.
   If not, write to the Free Software Foundation,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#import <WebKit/DOMCore.h>

@class DOMCSSStyleDeclaration;
@class WebDataSource;
@class WebFrame;
@class NSTextTable, NSTextTableBlock, NSTextList;	// we don't explicitly import since we can't rely on their existence
@class DOMHTMLElement;

@class _WebHTMLDocumentRepresentation;

@interface DOMElement (DOMHTMLElement)		// DOMElements also have tag attributes

// generic parser support and control

typedef enum 
{
	DOMHTMLNoNesting,			// character-like tags e.g. <img>, <hr>, <br>, <img>
	DOMHTMLStandardNesting,		// standard e.g. <b>, <ul>, <span>
	DOMHTMLLazyNesting,			// old HTML compatibility mode: a<p>b<p>c or <ul><li>a<li>b</ul> as a<p>b</p><p>c</p> or <ul><li>a</li><li>b</li></ul>
	DOMHTMLSingletonNesting,	// merge all into single node (e.g. <body>)
	DOMHTMLIgnore				// ignore node
} DOMHTMLNestingStyle;

+ (DOMHTMLNestingStyle) _nesting;		// controls building of tree
+ (DOMHTMLElement *) _designatedParentNode:(_WebHTMLDocumentRepresentation *) rep;			// return the parent node

- (void) _elementDidAwakeFromDocumentRepresentation:(_WebHTMLDocumentRepresentation *) rep;	// node has just been decoded but not processed otherwise
- (void) _elementLoaded;	// element has been loaded (i.e. tag was closed - not called for all nesting modes)

// HTML access

- (WebFrame *) webFrame;

// subresource loading

- (NSURL *) URLWithAttributeString:(NSString *) string;	// we don't inherit from DOMDocument...
- (NSData *) _loadSubresourceWithAttributeString:(NSString *) string blocking:(BOOL) flag;
- (void) _triggerEvent:(NSString *) event;

// generic rendering support


- (NSAttributedString *) _tableCellsForTable:(NSTextTable *) table row:(unsigned *) row col:(unsigned *) col;

@end

@interface DOMCharacterData (DOMHTMLElement)	// is not a subclass of DOMElement!

- (NSString *) outerHTML;
- (NSString *) innerHTML;

- (void) _spliceTo:(NSMutableAttributedString *) str;	// uses _style and _string to get content to splice - handles block/inline splicing rules 
- (void) _flushStyles;

- (void) _layout:(NSView *) parent;

@end

@interface DOMHTMLElement : DOMElement	// add private methods that need to work for HTML nodes only
{
	NSMutableDictionary *_style;	// cached attribute&CSS style
}

- (NSAttributedString *) attributedString;

- (NSString *) outerHTML;
- (void) setOuterHTML:(NSString *) str;	// this should parse HTML and replace the node type and the contents
- (NSString *) innerHTML;
- (void) setInnerHTML:(NSString *) str;	// this should parse HTML and replace the contents

- (void) _layout:(NSView *) view;		// layout view according to the DOM node (may swap the view within its superview!)
- (void) _spliceTo:(NSMutableAttributedString *) str;	// uses _style and _string to get content to splice - handles block/inline splicing rules 
- (NSMutableDictionary *) _style;				// get attributes (merging explicit attributes and CSS from this and parent levels)
- (void) _flushStyles;							// flush style cache
- (NSString *) _string;							// get string to be spliced
- (NSTextAttachment *) _attachment;				// get attachment to be spliced

- (void) _addCSSToStyle;						// add CSS to style
- (void) _addAttributesToStyle;					// add attributes to style

@end

@interface DOMHTMLDocument : RENAME(DOMDocument)	// the whole document
{
	WebDataSource *_dataSource;		// the datasource we belong to - not retained!
	WebFrame *_webFrame;			// the webframe we belong to - not retained!
}

- (void) _setWebFrame:(WebFrame *) frame;
- (WebFrame *) webFrame;
- (void) _setWebDataSource:(WebDataSource *) src;
- (WebDataSource *) _webDataSource;

@end

@interface DOMHTMLHtmlElement : DOMHTMLElement		// <html> - has <head> and <body> and <frameset> children
@end

@interface DOMHTMLHeadElement : DOMHTMLElement		// <head>
@end

@interface DOMHTMLTitleElement : DOMHTMLElement		// <title>
@end

@interface DOMHTMLMetaElement : DOMHTMLElement		// <meta>
@end

@interface DOMHTMLLinkElement : DOMHTMLElement		// <link>
@end

@interface DOMHTMLStyleElement : DOMHTMLElement		// <style>
@end

@interface DOMHTMLScriptElement : DOMHTMLElement	// <script>
@end

@interface DOMHTMLObjectElement : DOMHTMLElement	// <object>
@end

@interface DOMHTMLParamElement : DOMHTMLElement	// <param>
@end

@interface DOMHTMLFrameSetElement : DOMHTMLElement		// <frameset>
@end

@interface DOMHTMLFrameElement : DOMHTMLElement		// <frame>
@end

@interface DOMHTMLNoFramesElement : DOMHTMLFrameElement		// <noframes>
@end

@interface DOMHTMLIFrameElement : DOMHTMLFrameElement	// <iframe>
@end

@interface DOMHTMLObjectFrameElement : DOMHTMLFrameElement	// <applet> or <object>
@end

@interface DOMHTMLBodyElement : DOMHTMLElement	// <body>
@end

@interface DOMHTMLCenterElement : DOMHTMLElement	// <center>
@end

@interface DOMHTMLHeadingElement : DOMHTMLElement	// <h>
@end

@interface DOMHTMLPreElement : DOMHTMLElement	// <pre>
@end

@interface DOMHTMLDivElement : DOMHTMLElement	// <div>
@end

@interface DOMHTMLSpanElement : DOMHTMLElement	// <div>
@end

@interface DOMHTMLFontElement : DOMHTMLElement	// <font>
@end

@interface DOMHTMLAnchorElement : DOMHTMLElement	// <a>
@end

@interface DOMHTMLImageElement : DOMHTMLElement	// <img>
@end

@interface DOMHTMLBRElement : DOMHTMLElement	// <br>
@end

@interface DOMHTMLParagraphElement : DOMHTMLElement	// <p>
@end

@interface DOMHTMLHRElement : DOMHTMLElement	// <hr>
@end

@interface DOMHTMLTableElement : DOMHTMLElement	// <table>
{
	id table;
}
@end

@interface DOMHTMLTBodyElement : DOMHTMLElement	// <tbody>
@end

@interface DOMHTMLTableRowElement : DOMHTMLElement	// <tr>
@end

@interface DOMHTMLTableCellElement : DOMHTMLElement	// <td>
{
	id cell;
}
@end

@interface DOMHTMLFormElement : DOMHTMLElement	// <form>
@end

@interface DOMHTMLInputElement : DOMHTMLElement		// <input>
@end

@interface DOMHTMLButtonElement : DOMHTMLElement	// <button>
@end

@interface DOMHTMLSelectElement : DOMHTMLElement	// <select>
@end

@interface DOMHTMLOptionElement : DOMHTMLElement	// <option>
@end

@interface DOMHTMLOptGroupElement : DOMHTMLElement	// <optgroup>
@end

@interface DOMHTMLLabelElement : DOMHTMLElement		// <label>
@end

@interface DOMHTMLTextAreaElement : DOMHTMLElement	// <textarea>
@end

@interface DOMHTMLLIElement : DOMHTMLElement		// <li>, <dt>, <dd>
@end

@interface DOMHTMLDListElement : DOMHTMLElement		// <dl>
@end

@interface DOMHTMLOListElement : DOMHTMLElement		// <ol>
@end

@interface DOMHTMLUListElement : DOMHTMLElement		// <ul>
@end

@interface DOMHTMLCanvasElement : DOMHTMLElement	// <canvas>
@end

// plus some more...

// all other tags are plain DOMHTMLElements (e.g. <center>)

