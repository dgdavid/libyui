/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       NCPopupDeps.cc

   Author:     Gabriele Strattner <gs@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#include "Y2Log.h"
#include "NCPopupDeps.h"

#include "NCTree.h"
#include "YMenuButton.h"
#include "YDialog.h"
#include "NCSplit.h"
#include "NCSpacing.h"
#include "PkgNames.h"
#include "NCPkgTable.h"
#include "NCMenuButton.h"
#include "NCPushButton.h"
#include "ObjectStatStrategy.h"

using namespace std;

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::NCPopupDeps
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
NCPopupDeps::NCPopupDeps( const wpos at, PackageSelector * pkger )
    : NCPopup( at, false )
      , cancelButton( 0 )
      , okButton( 0 )
      , solveButton( 0 )
      , pkgs( 0 )
      , deps( 0 )
      , depsMenu( 0 )
      , packager( pkger )
{
    createLayout( PkgNames::PackageDeps() );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::~NCPopupDeps
//	METHOD TYPE : Destructor
//
//	DESCRIPTION :
//
NCPopupDeps::~NCPopupDeps()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::createLayout
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void NCPopupDeps::createLayout( const YCPString & headline )
{

  YWidgetOpt opt;

  // vertical split is the (only) child of the dialog
  NCSplit * vSplit = new NCSplit( this, opt, YD_VERT );
  addChild( vSplit );

  // opt.vWeight.setValue( 40 );

  opt.notifyMode.setValue( true );

  NCSpacing * sp0 = new NCSpacing( vSplit, opt, 0.2, false, true );
  vSplit->addChild( sp0 );

  // add the headline
  opt.isHeading.setValue( true );
  NCLabel * head = new NCLabel( vSplit, opt, headline );
  vSplit->addChild( head );

  NCSpacing * sp = new NCSpacing( vSplit, opt, 0.8, false, true );
  vSplit->addChild( sp );

  // add the list containing packages with unresolved depemdencies
  pkgs = new NCPkgTable( vSplit, opt );
  pkgs->setPackager( packager );
  // set status strategy
  ObjectStatStrategy * strategy =  new DependencyStatStrategy();
  pkgs->setTableType( NCPkgTable::T_Dependency, strategy );
  vSplit->addChild( pkgs );

  depsMenu = new NCMenuButton( vSplit, opt, YCPString( "Dependencies" ) );
  depsMenu->addMenuItem( PkgNames::RequiredBy(), PkgNames::RequRel() );
  depsMenu->addMenuItem( PkgNames::ConflictDeps(), PkgNames::ConflRel() );
  depsMenu->addMenuItem( PkgNames::Alternatives(), PkgNames::AlterRel() );

  vSplit->addChild( depsMenu );

  NCSpacing * sp1 = new NCSpacing( vSplit, opt, 0.8, false, true );
  vSplit->addChild( sp1 );

  // add the package list containing the dependencies
  deps = new NCPkgTable( vSplit, opt );
  deps->setPackager( packager );
  // set status strategy
  ObjectStatStrategy * strat = new DependencyStatStrategy();
  deps->setTableType( NCPkgTable::T_Dependency, strat );
  vSplit->addChild( deps );

  NCSplit * hSplit = new NCSplit( vSplit, opt, YD_HORIZ );
  vSplit->addChild( hSplit );

  // add the cancel button
  opt.isHStretchable.setValue( false );
  cancelButton = new NCPushButton( hSplit, opt, PkgNames::CancelLabel() );
  cancelButton->setId( PkgNames::Cancel () );
  hSplit->addChild( cancelButton );

  solveButton = new NCPushButton( hSplit, opt, PkgNames::SolveLabel() );
  solveButton->setId( PkgNames::Solve () );
  hSplit->addChild( solveButton );

  okButton = new NCPushButton( hSplit, opt, PkgNames::OKLabel() );
  okButton->setId( PkgNames::OkButton () );
  hSplit->addChild( okButton );

}

///////////////////////////////////////////////////////////////////
//
//  showDependencies
// 
//
void NCPopupDeps::showDependencies( )
{

    // 	typedef std::list<Result> ResultList;
    PkgDep::ResultList		goodList;
    
    //
    //  struct ErrorResult {
    // 	    RelInfoList unresolvable;
    //	    	
    //  }
    //	typedef std::list<RelInfo> RelInfoList;
    // 	struct RelInfo {
    //      // name of package causing the relation:
    //      PkgName name;
    //  }
    //

    //	typedef std::list<ErrorResult> ErrorResultList;
    PkgDep::ErrorResultList	badList;
   
    NCMIL << "Solving..." << endl ;

    // call the Y2PM::packageManager() to get the "badlist"
    bool success = Y2PM::packageManager().solveInstall( goodList, badList );

    if ( !success )
    {
	// evaluate the ErrorResultList
	evaluateErrorResult( badList );

        // clear the lists
	deps->itemsCleared();
	pkgs->itemsCleared();
	
	// fill the list with packages  which have unresolved deps
	fillDepsPackageList( pkgs );

	showDependencyPopup();    // show the dependencies
    }
    
}


void NCPopupDeps::evaluateErrorResult( PkgDep::ErrorResultList errorlist )
{
    int i = 0;
    PkgDep::ErrorResultList::iterator it = errorlist.begin();
    
    while ( it != errorlist.end() )
    {
	if ( (*it).solvable || !(*it).alternatives.empty() )
	{
	    // save (*it) is an ErrorResult
	    dependencies.push_back( *it );
	}
	else 
	{
	    NCMIL << "No PMSolvablePtr for " << (*it) << endl;
	}
	
	++it;
	i++;
    }

}


bool NCPopupDeps::fillDepsPackageList( NCPkgTable * table )
{
    if ( !table )
	return false;
    
    // create the list of "bad" packages
    vector< PkgDep::ErrorResult>::iterator it = dependencies.begin();
    unsigned int i = 0;
    vector<string> pkgLine;
    pkgLine.reserve(4);
    table->itemsCleared ();
     
    while ( it != dependencies.end() )
    {
	PMObjectPtr objPtr = (*it).solvable;
	if ( objPtr )
	{
	    pkgLine.clear();
	    pkgLine.push_back( objPtr->getSelectable()->name() );	// package name
	    string kind = getDependencyKind( (*it) );
	    pkgLine.push_back( kind );
	    pkgLine.push_back( objPtr->summary() );

	    table->addLine( objPtr->getSelectable()->status(), //  get the package status
			    pkgLine,
			    i,		// the index
			    objPtr );	// the corresponding package pointer
	
	}
	++it;
	i++;
    }

    return true;
}


string NCPopupDeps::getDependencyKind(  PkgDep::ErrorResult error )
{
    string ret;
    
    if ( !error.unresolvable.empty() )
    {
	NCMIL << "UNRESOLVABLE " << error.unresolvable << endl;
	ret = "UNRESOLVABLE";
    }
    if ( !error.alternatives.empty() )
    {
	NCMIL << "ALTERNATIVES " << error.alternatives << endl;
	ret = "ALTERNATIVES";
    }
    if ( !error.conflicts_with.empty() )
    {
	NCMIL << "CONFLICTS: " << error.conflicts_with << endl;
	ret = "CONFLICT";
	if ( !error.remove_to_solve_conflict.empty() )
	{
	    NCMIL << "REMOVE to solve not empty" << endl;
	}
    }

    return ret;
}

void NCPopupDeps::concretelyDependency( int index )
{
    unsigned int size = dependencies.size();
    
    if ( index >= 0 && (unsigned int)index < size )
    {
	PkgDep::ErrorResult error = dependencies[index];
	NCMIL << "Showing: " << error << endl;	
    } 

}


///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::showDependencyPopup
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
NCursesEvent NCPopupDeps::showDependencyPopup( )
{
    postevent = NCursesEvent();

    do {
	popupDialog();
	
    } while ( postAgain() );

    popdownDialog();

    return postevent;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::niceSize
//	METHOD TYPE : void
//
//	DESCRIPTION :
//

long NCPopupDeps::nicesize(YUIDimension dim)
{
    return ( dim == YD_HORIZ ? 50 : 20 );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopup::wHandleInput
//	METHOD TYPE : NCursesEvent
//
//	DESCRIPTION :
//
NCursesEvent NCPopupDeps::wHandleInput( int ch )
{
    if ( ch == 27 ) // ESC
	return NCursesEvent::cancel;

    return NCDialog::wHandleInput( ch );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupDeps::postAgain
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool NCPopupDeps::postAgain()
{
    if ( ! postevent.widget )
	return false;

    YCPValue currentId =  dynamic_cast<YWidget *>(postevent.widget)->id();

    if ( currentId->compare( PkgNames::Cancel () ) == YO_EQUAL )
    {
	return false;
    }
    else if  ( currentId->compare( PkgNames::Solve () ) == YO_EQUAL )
    {
	// get currently selected package
	PMObjectPtr currentPtr = pkgs->getDataPointer( pkgs->getCurrentItem() );

	// solve the dependencies and create new list of "bad" packages
	// checkDependencies();
	
	// set current item ( if the package is still there )
	if ( currentPtr )
	{
	    unsigned int size = pkgs->getNumLines();
	    unsigned int index = 0;
	    PMObjectPtr pkgPtr;
	    while ( index < size )
	    {
		pkgPtr = pkgs->getDataPointer( index );
		if ( pkgPtr == currentPtr )
		{
		    NCMIL << "Setting current package line: " << index << endl;
		    pkgs->setCurrentItem( index );
		    break;
		}
		index ++;
	    }
	}

	pkgs->setKeyboardFocus();
    }
    else if  ( currentId->compare( PkgNames::OkButton () ) == YO_EQUAL )
    {
	return false;
    }

    if ( postevent == NCursesEvent::cancel )
    {
	// return false means: close the popup dialog
	return false;
    }
    return true;
}

