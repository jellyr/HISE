/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licenses for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licensing:
*
*   http://www.hise.audio/
*
*   HISE is based on the JUCE library,
*   which must be separately licensed for cloused source applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#define toggleVisibility(x) {x->setVisible(!x->isVisible()); owner->setComponentShown(info.commandID, x->isVisible());}

#define SET_COMMAND_TARGET(result, name, active, ticked, shortcut) { result.setInfo(name, name, "Target", 0); \\
															 result.setActive(active); \\
															 result.setTicked(ticked); \\
															 result.addDefaultKeypress(shortcut, ModifierKeys::commandModifier); }



#define ADD_ALL_PLATFORMS(x)(p.addCommandItem(mainCommandManager, x))

#if HISE_IOS
#define ADD_IOS_ONLY(x)(p.addCommandItem(mainCommandManager, x))
#define ADD_DESKTOP_ONLY(x)
#else
#define ADD_IOS_ONLY(x)()
#define ADD_DESKTOP_ONLY(x)(p.addCommandItem(mainCommandManager, x))
#endif


BackendCommandTarget::BackendCommandTarget(BackendProcessor *owner_):
owner(owner_),
currentColumnMode(OneColumn)
{
	

	createMenuBarNames();
}


void BackendCommandTarget::setEditor(BackendRootWindow *editor)
{
	bpe = dynamic_cast<BackendRootWindow*>(editor);

	mainCommandManager = owner->getCommandManager();
	mainCommandManager->registerAllCommandsForTarget(this);
	mainCommandManager->getKeyMappings()->resetToDefaultMappings();

	bpe->addKeyListener(mainCommandManager->getKeyMappings());
	mainCommandManager->setFirstCommandTarget(this);
	mainCommandManager->commandStatusChanged();
}

void BackendCommandTarget::getAllCommands(Array<CommandID>& commands)
{
	const CommandID id[] = { 
		Settings,
		WorkspaceMain,
		WorkspaceScript,
		WorkspaceSampler,
		WorkspaceCustom,
		MenuNewFile,
		MenuOpenFile,
		MenuSaveFile,
		MenuSaveFileAs,
		MenuSaveFileAsXmlBackup,
		MenuOpenXmlBackup,
		MenuProjectNew,
		MenuProjectLoad,
		MenuCloseProject,
		MenuFileArchiveProject,
		MenuFileDownloadNewProject,
		MenuProjectShowInFinder,
        MenuFileSaveUserPreset,
		MenuFileSettingsPreset,
		MenuFileSettingsProject,
		MenuFileSettingsUser,
		MenuFileSettingsCompiler,
		MenuFileSettingCheckSanity,
		MenuFileSettingsCleanBuildDirectory,
		MenuReplaceWithClipboardContent,
		MenuExportFileAsPlugin,
		MenuExportFileAsEffectPlugin,
		MenuExportFileAsStandaloneApp,
		MenuExportFileAsPlayerLibrary,
		MenuExportFileAsSnippet,
		MenuFileQuit,
		MenuEditUndo,
		MenuEditRedo,
		MenuEditCopy,
		MenuEditPaste,
		MenuEditMoveUp,
		MenuEditMoveDown,
		MenuEditCreateScriptVariable,
		MenuEditCreateBase64State,
        MenuEditCloseAllChains,
        MenuEditPlotModulator,
		MenuViewShowSelectedProcessorInPopup,
		
		MenuToolsRecompile,
		MenuToolsCreateInterface,
        MenuToolsClearConsole,
		MenuToolsSetCompileTimeOut,
		MenuToolsUseBackgroundThreadForCompile,
		MenuToolsRecompileScriptsOnReload,
		MenuToolsCreateToolbarPropertyDefinition,
		MenuToolsCreateExternalScriptFile,
		MenuToolsValidateUserPresets,
		MenuToolsResolveMissingSamples,
		MenuToolsDeleteMissingSamples,
		MenuToolsCheckAllSampleMaps,
		MenuToolsUseRelativePaths,
		MenuToolsCollectExternalFiles,
        MenuToolsRedirectSampleFolder,
		MenuToolsForcePoolSearch,
		MenuToolsConvertAllSamplesToMonolith,
		MenuToolsConvertSfzToSampleMaps,
		MenuToolsEnableAutoSaving,
		MenuToolsEnableDebugLogging,
		MenuToolsCreateRSAKeys,
		MenuToolsCreateDummyLicenceFile,
		MenuViewReset,
        MenuViewFullscreen,
		MenuViewBack,
		MenuViewForward,
		MenuViewEnableGlobalLayoutMode,
        MenuOneColumn,
		MenuTwoColumns,
		MenuThreeColumns,
		MenuViewShowPluginPopupPreview,
        MenuViewIncreaseCodeFontSize,
        MenuViewDecreaseCodeFontSize,
        MenuAddView,
        MenuDeleteView,
        MenuRenameView,
        MenuViewSaveCurrentView,
        MenuToolsCheckDuplicate,
		MenuHelpShowAboutPage,
        MenuHelpCheckVersion
		
	};

	commands.addArray(id, numElementsInArray(id));
}

void BackendCommandTarget::createMenuBarNames()
{
	menuNames.clear();

	menuNames.add("File");
	menuNames.add("Edit " + (currentCopyPasteTarget.get() == nullptr ? "" : currentCopyPasteTarget->getObjectTypeName()));
	menuNames.add("Tools");
	menuNames.add("View");
	menuNames.add("Help");

	jassert(menuNames.size() == numMenuNames);
}

void BackendCommandTarget::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case HamburgerMenu:
		setCommandTarget(result, "Show Menu", true, false, 'X', false);
		break;
	case Settings:

#if IS_STANDALONE_APP
		setCommandTarget(result, "Show Audio Device Settings", true, false, 'X', false);
#else
		setCommandTarget(result, "Show Audio Device Settings (disabled for plugins)", false, bpe->currentDialog == nullptr, '8');
#endif
		break;

	case WorkspaceMain:
	{
		setCommandTarget(result, "Show Main Workspace", true, bpe->getCurrentWorkspace() == WorkspaceMain, 'X', false);
		break;
	}
	case WorkspaceScript:
	{
		setCommandTarget(result, "Show Scripting Workspace", true, bpe->getCurrentWorkspace() == WorkspaceScript, 'X', false);
		break;
	}
	case WorkspaceSampler:
	{
		setCommandTarget(result, "Show Sampler Workspace", true, bpe->getCurrentWorkspace() == WorkspaceSampler, 'X', false);
		break;
	}
	case WorkspaceCustom:
	{
		setCommandTarget(result, "Show Custom Workspace", true, bpe->getCurrentWorkspace() == WorkspaceCustom, 'X', false);
		break;
	}
	case MenuNewFile:
		setCommandTarget(result, "New File", true, false, 'N');
		break;
	case MenuOpenFile:
		setCommandTarget(result, "Open File", true, false, 'O');
		break;
	case MenuSaveFile:
		setCommandTarget(result, "Save", true, false, 'S');
		break;
	case MenuSaveFileAs:
		setCommandTarget(result, "Save As", true, false, 'X', false);
		break;
	case MenuSaveFileAsXmlBackup:
		setCommandTarget(result, "Save File as XML Backup", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
	case MenuOpenXmlBackup:
		setCommandTarget(result, "Open XML Backup", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
	case MenuProjectNew:
		setCommandTarget(result, "Create new Project folder", true, false, 'X', false);
		break;
	case MenuProjectLoad:
		setCommandTarget(result, "Load Project", true, false, 'X', false);
		break;
	case MenuCloseProject:
		setCommandTarget(result, "Close Project", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
	case MenuFileArchiveProject:
		setCommandTarget(result, "Archive Project", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
	case MenuFileDownloadNewProject:
		setCommandTarget(result, "Download archived Project", true, false, 'X', false);
		break;
	case MenuProjectShowInFinder:
#if JUCE_WINDOWS
        setCommandTarget(result, "Show Project folder in Explorer",
#else
		setCommandTarget(result, "Show Project folder in Finder",
#endif
        GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
    case MenuFileSaveUserPreset:
        setCommandTarget(result, "Save current state as new User Preset", true, false, 'X', false);
        break;
    case MenuExportFileAsPlugin:
        setCommandTarget(result, "Export as Instrument (VSTi / AUi) plugin", true, false, 'X', false);
        break;
	case MenuExportFileAsEffectPlugin:
		setCommandTarget(result, "Export as FX plugin", true, false, 'X', false);
		break;
	case MenuExportFileAsStandaloneApp:
		setCommandTarget(result, "Export as Standalone Application", true, false, 'X', false);
		break;
	case MenuExportFileAsPlayerLibrary:
		setCommandTarget(result, "Export as HISE Player Library", true, false, 'X', false);
		break;
	case MenuExportFileAsSnippet:
		setCommandTarget(result, "Export as pasteable web snippet", true, false, 'X', false);
		break;
	case MenuFileSettingsPreset:
		setCommandTarget(result, "Preset Properties", true, false, 'X', false);
		break;
	case MenuFileSettingsProject:
		setCommandTarget(result, "Project Properties", true, false, 'X', false);
		break;
	case MenuFileSettingsUser:
		setCommandTarget(result, "User Settings", true, false, 'X', false);
		break;
	case MenuFileSettingsCompiler:
		setCommandTarget(result, "Compiler Settings", true, false, 'X', false);
		break;
	case MenuFileSettingCheckSanity:
		setCommandTarget(result, "Check for missing properties", true, false, 'X', false);
		break;
	case MenuFileSettingsCleanBuildDirectory:
		setCommandTarget(result, "Clean Build directory", true, false, 'X', false);
		break;
	case MenuReplaceWithClipboardContent:
		setCommandTarget(result, "Replace with clipboard content", true, false, 'X', false);
		break;
	case MenuFileQuit:
		setCommandTarget(result, "Quit", true, false, 'X', false); break;
	case MenuEditUndo:
		setCommandTarget(result, "Undo: " + bpe->owner->getControlUndoManager()->getUndoDescription(), bpe->owner->getControlUndoManager()->canUndo(), false, 'Z', true, ModifierKeys::commandModifier);
		break;
	case MenuEditRedo:
		setCommandTarget(result, "Redo: " + bpe->owner->getControlUndoManager()->getRedoDescription(), bpe->owner->getControlUndoManager()->canRedo(), false, 'Y', true, ModifierKeys::commandModifier);
		break;
	case MenuEditCopy:
		setCommandTarget(result, "Copy", currentCopyPasteTarget.get() != nullptr, false, 'C');
		break;
	case MenuEditPaste:
		setCommandTarget(result, "Paste", currentCopyPasteTarget.get() != nullptr, false, 'V');
		break;
	case MenuEditMoveUp:
		setCommandTarget(result, "Move up", currentCopyPasteTarget.get() != nullptr, false, 'X', false);
		result.addDefaultKeypress(KeyPress::upKey, ModifierKeys::ctrlModifier);
		break;
	case MenuEditMoveDown:
		setCommandTarget(result, "Move down", currentCopyPasteTarget.get() != nullptr, false, 'X', false);
		result.addDefaultKeypress(KeyPress::downKey, ModifierKeys::ctrlModifier);
		break;
	case MenuEditCreateScriptVariable:
		setCommandTarget(result, "Create script variable", clipBoardNotEmpty(), false, 'C', true, ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
		break;
	case MenuEditCreateBase64State:
		setCommandTarget(result, "Create Base64 encoded state", clipBoardNotEmpty(), false, 'C', false);
		break;
    case MenuEditPlotModulator:
        {
            ProcessorEditor * editor = dynamic_cast<ProcessorEditor*>(currentCopyPasteTarget.get());
            
            bool active = false;
            
            bool ticked = false;
            
            if(editor != nullptr)
            {
                Modulator *mod = dynamic_cast<Modulator*>(editor->getProcessor());
                
                if(mod != nullptr)
                {
                    active = true;
                    
                    ticked = mod->isPlotted();
                }
            }
            
            setCommandTarget(result, "Plot Modulator", active, ticked, 'P');
            break;
        }

    case MenuEditCloseAllChains:
        setCommandTarget(result, "Close all chains", clipBoardNotEmpty(), false, 'X', false);
        break;
	case MenuToolsCreateInterface:
		setCommandTarget(result, "Create User Interface", true, false, 'X', false);
		break;
	case MenuToolsRecompile:
                         setCommandTarget(result, "Recompile all scripts", true, false, 'X', false);
                         result.addDefaultKeypress(KeyPress::F5Key, ModifierKeys::shiftModifier);
        break;
	case MenuToolsSetCompileTimeOut:
		setCommandTarget(result, "Change compile time out duration", true, false, 'X', false);
		break;
	case MenuToolsUseBackgroundThreadForCompile:
		setCommandTarget(result, "Use background thread for script compiling", true, bpe->getBackendProcessor()->isUsingBackgroundThreadForCompiling(), 'X', false);
		break;
	case MenuToolsRecompileScriptsOnReload:
		setCommandTarget(result, "Recompile all scripts on preset load", true, bpe->getBackendProcessor()->isCompilingAllScriptsOnPresetLoad(), 'X', false);
		break;
	case MenuToolsCreateToolbarPropertyDefinition:
		setCommandTarget(result, "Create default Toolbar JSON definition", true, false, 'X', false);
		break;
	case MenuToolsCreateExternalScriptFile:
		setCommandTarget(result, "Create external script file", true, false, 'X', false);
		break;
	case MenuToolsValidateUserPresets:
		setCommandTarget(result, "Validate User Presets", true, false, 'X', false);
		break;
	case MenuToolsDeleteMissingSamples:
		setCommandTarget(result, "Delete missing samples", true, false, 'X', false);
		break;
	case MenuToolsResolveMissingSamples:
		setCommandTarget(result, "Resolve missing samples", true, false, 'X', false);
		break;
	case MenuToolsUseRelativePaths:
		setCommandTarget(result, "Use relative paths to project folder", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), bpe->getBackendProcessor()->getSampleManager().shouldUseRelativePathToProjectFolder(), 'X', false);
		break;
	case MenuToolsCheckAllSampleMaps:
		setCommandTarget(result, "Check all sample maps", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
	case MenuToolsCollectExternalFiles:
		setCommandTarget(result, "Collect external files into Project folder", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(), false, 'X', false);
		break;
    case MenuToolsRedirectSampleFolder:
		setCommandTarget(result, "Redirect sample folder", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive(),
			GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isRedirected(ProjectHandler::SubDirectories::Samples), 'X', false);
        break;
	case MenuToolsForcePoolSearch:
		setCommandTarget(result, "Force duplicate search in pool when loading samples", true, bpe->getBackendProcessor()->getSampleManager().getModulatorSamplerSoundPool()->isPoolSearchForced(), 'X', false);
		break;
	case MenuToolsConvertAllSamplesToMonolith:
		setCommandTarget(result, "Convert all samples to Monolith + Samplemap", true, false, 'X', false);
		break;
	case MenuToolsConvertSfzToSampleMaps:
		setCommandTarget(result, "Convert SFZ files to SampleMaps", true, false, 'X', false);
		break;
	case MenuToolsEnableAutoSaving:
		setCommandTarget(result, "Enable Autosaving", true, bpe->owner->getAutoSaver().isAutoSaving(), 'X', false);
		break;
	case MenuToolsEnableDebugLogging:
		setCommandTarget(result, "Enable Debug Logger", true, bpe->owner->getDebugLogger().isLogging(), 'X', false);
		break;
	case MenuToolsCreateRSAKeys:
		setCommandTarget(result, "Create RSA Key pair", true, false, 'X', false);
		break;
	case MenuToolsCreateDummyLicenceFile:
		setCommandTarget(result, "Create Dummy Licence File", true, false, 'X', false);
		break;
	case MenuViewReset:
		setCommandTarget(result, "Reset Workspaces", true, false, 'X', false);
		break;
    case MenuViewFullscreen:
        setCommandTarget(result, "Toggle Fullscreen", true, bpe->isFullScreenMode(), 'F');
        break;
	case MenuViewBack:
		setCommandTarget(result, "Back: " + bpe->mainEditor->getViewUndoManager()->getUndoDescription(), bpe->mainEditor->getViewUndoManager()->canUndo(), false, (char)KeyPress::backspaceKey, true, ModifierKeys::noModifiers);
		break;
	case MenuViewForward:
		setCommandTarget(result, "Forward: " + bpe->mainEditor->getViewUndoManager()->getRedoDescription(), bpe->mainEditor->getViewUndoManager()->canRedo(), false, (char)KeyPress::backspaceKey, true, ModifierKeys::shiftModifier);
		break;
	case MenuViewEnableGlobalLayoutMode:
		setCommandTarget(result, "Enable Layout Mode", true, bpe->getRootFloatingTile()->isLayoutModeEnabled(), 'X', false);
		result.addDefaultKeypress(KeyPress::F6Key, ModifierKeys::noModifiers);
		break;
	case MenuOneColumn:
		setCommandTarget(result, "One Column", true, currentColumnMode == OneColumn, '1', true, ModifierKeys::altModifier);
		break;
	case MenuTwoColumns:
		setCommandTarget(result, "Two Columns", true, currentColumnMode == TwoColumns, '2', true, ModifierKeys::altModifier);
		break;
	case MenuThreeColumns:
		setCommandTarget(result, "Three Columns", true, currentColumnMode == ThreeColumns, '3', true, ModifierKeys::altModifier);
		break;
	case MenuViewShowPluginPopupPreview:
		setCommandTarget(result, "Open Plugin Preview Window", bpe->mainEditor->isPluginPreviewCreatable(), !bpe->mainEditor->isPluginPreviewShown(), 'X', false);
		break;
    case MenuViewIncreaseCodeFontSize:
        setCommandTarget(result, "Increase code font size", true, false, 'X', false);
        break;
    case MenuViewDecreaseCodeFontSize:
        setCommandTarget(result, "Decrease code font size", true, false, 'X', false);
        break;
    case MenuAddView:
        setCommandTarget(result, "Add new view", true, false, 'X', false);
        break;
    case MenuDeleteView:
        setCommandTarget(result, "Delete current view", viewActive(), false, 'X', false);
        break;
    case MenuRenameView:
        setCommandTarget(result, "Rename current view", viewActive(), false, 'X', false);
        break;
    case MenuViewSaveCurrentView:
        setCommandTarget(result, "Save current view", viewActive(), false, 'X', false);
        break;
	case MenuViewShowSelectedProcessorInPopup:
		setCommandTarget(result, "Show Processor in full screen", dynamic_cast<ProcessorEditor*>(currentCopyPasteTarget.get()) != nullptr, false, 'X', false);
		result.addDefaultKeypress(KeyPress::F11Key, ModifierKeys::noModifiers);
		break;
    case MenuToolsCheckDuplicate:
        setCommandTarget(result, "Check duplicate IDs", true, false, 'X', false);
        break;
    case MenuToolsClearConsole:
        setCommandTarget(result, "Clear Console", true, false, 'X', false);
        break;
	case MenuHelpShowAboutPage:
		setCommandTarget(result, "About HISE", true, false, 'X', false);

		break;
    case MenuHelpCheckVersion:
        setCommandTarget(result, "Check for newer version", true, false, 'X', false);
        break;
            
	default:					jassertfalse; return;
	}
}

bool BackendCommandTarget::perform(const InvocationInfo &info)
{
	CompileExporter exporter(bpe->getMainSynthChain());

	switch (info.commandID)
	{
	case HamburgerMenu:					Actions::showMainMenu(bpe);  return true;
	case Settings:                      bpe->showSettingsWindow(); return true;
	case WorkspaceMain:
	case WorkspaceScript:
	case WorkspaceSampler:
	case WorkspaceCustom:				bpe->showWorkspace(info.commandID); updateCommands(); return true;
	case MenuNewFile:                   if (PresetHandler::showYesNoWindow("New File", "Do you want to start a new preset?"))
                                            bpe->mainEditor->clearPreset(); return true;
	case MenuOpenFile:                  Actions::openFile(bpe); return true;
	case MenuSaveFile:                  Actions::saveFile(bpe, false); updateCommands(); return true;
	case MenuSaveFileAs:				Actions::saveFile(bpe, true); updateCommands(); return true;
    case MenuSaveFileAsXmlBackup:		Actions::saveFileAsXml(bpe); updateCommands(); return true;
    case MenuOpenXmlBackup:             { FileChooser fc("Select XML file to load",
                                                         GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::XMLPresetBackups), "*.xml", true);
                                        if (fc.browseForFileToOpen()) Actions::openFileFromXml(bpe, fc.getResult()); return true;}
	case MenuProjectNew:				Actions::createNewProject(bpe); updateCommands();  return true;
	case MenuProjectLoad:				Actions::loadProject(bpe); updateCommands(); return true;
	case MenuCloseProject:				Actions::closeProject(bpe); updateCommands(); return true;
	case MenuFileArchiveProject:		Actions::archiveProject(bpe); return true;
	case MenuFileDownloadNewProject:	Actions::downloadNewProject(bpe); return true;
	case MenuProjectShowInFinder:		Actions::showProjectInFinder(bpe); return true;
    case MenuFileSaveUserPreset:        Actions::saveUserPreset(bpe); return true;
	case MenuFileSettingsPreset:		Actions::showFilePresetSettings(bpe); return true;
	case MenuFileSettingsProject:		Actions::showFileProjectSettings(bpe); return true;
	case MenuFileSettingsUser:			Actions::showFileUserSettings(bpe); return true;
	case MenuFileSettingsCompiler:		Actions::showFileCompilerSettings(bpe); return true;
	case MenuFileSettingCheckSanity:	Actions::checkSettingSanity(bpe); return true;
	case MenuFileSettingsCleanBuildDirectory:	Actions::cleanBuildDirectory(bpe); return true;
	case MenuReplaceWithClipboardContent: Actions::replaceWithClipboardContent(bpe); return true;
	case MenuFileQuit:                  if (PresetHandler::showYesNoWindow("Quit Application", "Do you want to quit?"))
                                            JUCEApplicationBase::quit(); return true;
	case MenuEditUndo:					bpe->owner->getControlUndoManager()->undo(); updateCommands(); return true;
	case MenuEditRedo:					bpe->owner->getControlUndoManager()->redo(); updateCommands(); return true;
	case MenuEditCopy:                  if (currentCopyPasteTarget) currentCopyPasteTarget->copyAction(); return true;
	case MenuEditPaste:                 if (currentCopyPasteTarget) currentCopyPasteTarget->pasteAction(); return true;
	case MenuEditMoveUp:				if (currentCopyPasteTarget) Actions::moveModule(currentCopyPasteTarget, true); return true;
	case MenuEditMoveDown:				if (currentCopyPasteTarget) Actions::moveModule(currentCopyPasteTarget, false); return true;
    case MenuEditCreateScriptVariable:  Actions::createScriptVariableDeclaration(currentCopyPasteTarget); return true;
	case MenuEditCreateBase64State:		Actions::createBase64State(currentCopyPasteTarget.get()); return true;
    case MenuEditPlotModulator:         Actions::plotModulator(currentCopyPasteTarget.get()); updateCommands(); return true;
    case MenuEditCloseAllChains:        Actions::closeAllChains(bpe); return true;
	case MenuToolsRecompile:            Actions::recompileAllScripts(bpe); return true;
	case MenuToolsCreateInterface:		Actions::createUserInterface(bpe); return true;
	case MenuToolsSetCompileTimeOut:	Actions::setCompileTimeOut(bpe); return true;
	case MenuToolsUseBackgroundThreadForCompile: Actions::toggleUseBackgroundThreadsForCompiling(bpe); updateCommands(); return true;
	case MenuToolsRecompileScriptsOnReload: Actions::toggleCompileScriptsOnPresetLoad(bpe); updateCommands(); return true;
	case MenuToolsCreateToolbarPropertyDefinition:	Actions::createDefaultToolbarJSON(bpe); return true;
	case MenuToolsCreateExternalScriptFile:	Actions::createExternalScriptFile(bpe); updateCommands(); return true;
    case MenuToolsCheckDuplicate:       Actions::checkDuplicateIds(bpe); return true;
	case MenuToolsValidateUserPresets:	Actions::validateUserPresets(bpe); return true;
	case MenuToolsDeleteMissingSamples: Actions::deleteMissingSamples(bpe); return true;
	case MenuToolsResolveMissingSamples:Actions::resolveMissingSamples(bpe); return true;
	case MenuToolsUseRelativePaths:		Actions::toggleRelativePath(bpe); updateCommands();  return true;
	case MenuToolsCollectExternalFiles:	Actions::collectExternalFiles(bpe); return true;
    case MenuToolsRedirectSampleFolder: Actions::redirectSampleFolder(bpe->getMainSynthChain()); updateCommands(); return true;
	case MenuToolsForcePoolSearch:		Actions::toggleForcePoolSearch(bpe); updateCommands(); return true;
	case MenuToolsConvertAllSamplesToMonolith:	Actions::convertAllSamplesToMonolith(bpe); return true;
	case MenuToolsConvertSfzToSampleMaps:	Actions::convertSfzFilesToSampleMaps(bpe); return true;
	case MenuToolsCreateRSAKeys:		Actions::createRSAKeys(bpe); return true;
	case MenuToolsCreateDummyLicenceFile: Actions::createDummyLicenceFile(bpe); return true;
	case MenuToolsCheckAllSampleMaps:	Actions::checkAllSamplemaps(bpe); return true;
	case MenuToolsEnableAutoSaving:		bpe->owner->getAutoSaver().toggleAutoSaving(); updateCommands(); return true;
	case MenuToolsEnableDebugLogging:	bpe->owner->getDebugLogger().toggleLogging(), updateCommands(); return true;
    case MenuViewFullscreen:            Actions::toggleFullscreen(bpe); updateCommands(); return true;
	case MenuViewBack:					bpe->mainEditor->getViewUndoManager()->undo(); updateCommands(); return true;
	case MenuViewReset:				    bpe->resetInterface(); updateCommands(); return true;
	case MenuViewForward:				bpe->mainEditor->getViewUndoManager()->redo(); updateCommands(); return true;
	case MenuViewEnableGlobalLayoutMode: bpe->getRootFloatingTile()->setLayoutModeEnabled(!bpe->getRootFloatingTile()->isLayoutModeEnabled()); updateCommands(); return true;
	case MenuViewShowPluginPopupPreview: Actions::togglePluginPopupWindow(bpe); updateCommands(); return true;
    case MenuViewIncreaseCodeFontSize:  Actions::changeCodeFontSize(bpe, true); return true;
    case MenuViewDecreaseCodeFontSize:   Actions::changeCodeFontSize(bpe, false); return true;
	case MenuExportFileAsPlugin:        exporter.exportMainSynthChainAsInstrument(); return true;
	case MenuExportFileAsEffectPlugin:	exporter.exportMainSynthChainAsFX(); return true;
	case MenuExportFileAsStandaloneApp: exporter.exportMainSynthChainAsStandaloneApp(); return true;
    case MenuExportFileAsSnippet:       Actions::exportFileAsSnippet(bpe); return true;
	case MenuExportFileAsPlayerLibrary: Actions::exportMainSynthChainAsPlayerLibrary(bpe); return true;
    case MenuAddView:                   Actions::addView(bpe); updateCommands();return true;
    case MenuDeleteView:                Actions::deleteView(bpe); updateCommands();return true;
    case MenuRenameView:                Actions::renameView(bpe); updateCommands();return true;
    case MenuViewSaveCurrentView:       Actions::saveView(bpe); updateCommands(); return true;
    case MenuToolsClearConsole:         owner->getConsoleHandler().clearConsole(); return true;
	case MenuHelpShowAboutPage:			Actions::showAboutPage(bpe); return true;
    case MenuHelpCheckVersion:          Actions::checkVersion(bpe); return true;
	case MenuOneColumn:					Actions::setColumns(bpe, this, OneColumn);  updateCommands(); return true;
	case MenuTwoColumns:				Actions::setColumns(bpe, this, TwoColumns);  updateCommands(); return true;
	case MenuThreeColumns:				Actions::setColumns(bpe, this, ThreeColumns);  updateCommands(); return true;
	}

	return false;
}



PopupMenu BackendCommandTarget::getMenuForIndex(int topLevelMenuIndex, const String &/*menuName*/)
{
	MenuNames m = (MenuNames)topLevelMenuIndex;

	PopupMenu p;

	switch (m)
	{
	case BackendCommandTarget::FileMenu: {
		ADD_ALL_PLATFORMS(MenuNewFile);

		ADD_DESKTOP_ONLY(MenuOpenFile);
		ADD_ALL_PLATFORMS(MenuSaveFile);
		ADD_ALL_PLATFORMS(MenuSaveFileAs);
		ADD_ALL_PLATFORMS(MenuReplaceWithClipboardContent);

		PopupMenu filesInProject;

		if (GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive())
		{
			GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getFileList(recentFileList, ProjectHandler::SubDirectories::Presets, "*.hip", true);

			for (int i = 0; i < recentFileList.size(); i++)
			{
				filesInProject.addItem(MenuOpenFileFromProjectOffset+i, recentFileList[i].getFileNameWithoutExtension(), true, false);
			}
		}

		p.addSubMenu("Open File from Project", filesInProject, filesInProject.getNumItems() != 0);

		p.addSeparator();

		ADD_ALL_PLATFORMS(MenuProjectNew);
		ADD_DESKTOP_ONLY(MenuProjectLoad);
		ADD_DESKTOP_ONLY(MenuCloseProject);
		ADD_DESKTOP_ONLY(MenuProjectShowInFinder);

		PopupMenu recentProjects;

#if HISE_IOS

		Array<File> results;

		File userDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory);

		userDataDirectory.findChildFiles(results, File::findDirectories, false);

		String currentProject = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getWorkDirectory().getFullPathName();
		
		const String menuTitle = "Available Projects";

		for (int i = 0; i < results.size(); i++)
		{
			recentProjects.addItem(MenuProjectRecentOffset + i, results[i].getFileName(), true, results[i].getFullPathName() == currentProject);
		}

#else

		StringArray recentProjectDirectories = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getRecentWorkDirectories();

		const String menuTitle = "Recent Projects";

		String currentProject = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getWorkDirectory().getFullPathName();

		for (int i = 0; i < recentProjectDirectories.size(); i++)
		{
			recentProjects.addItem(MenuProjectRecentOffset + i, recentProjectDirectories[i], true, currentProject == recentProjectDirectories[i]);
		}

#endif

		p.addSubMenu(menuTitle, recentProjects);
        
        p.addSeparator();

		ADD_DESKTOP_ONLY(MenuFileArchiveProject);
		ADD_ALL_PLATFORMS(MenuFileDownloadNewProject);

		p.addSeparator();

        ADD_ALL_PLATFORMS(MenuFileSaveUserPreset);
        
        PopupMenu userPresets;
        Array<File> userPresetFiles;
        
        ProjectHandler *handler = &GET_PROJECT_HANDLER(bpe->getMainSynthChain());
        
        handler->getFileList(userPresetFiles, ProjectHandler::SubDirectories::UserPresets, "*.preset");
            
        for(int i = 0; i < userPresetFiles.size(); i++)
            userPresets.addItem(i + MenuFileUserPresetMenuOffset, userPresetFiles[i].getFileName());
        
        
        p.addSubMenu("User Presets", userPresets);
        p.addSeparator();
        
		ADD_ALL_PLATFORMS(MenuOpenXmlBackup);
		ADD_ALL_PLATFORMS(MenuSaveFileAsXmlBackup);

        PopupMenu xmlBackups;
        Array<File> xmlBackupFiles;
        
		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getFileList(xmlBackupFiles, ProjectHandler::SubDirectories::XMLPresetBackups, "*.xml");
        
        for(int i = 0; i < xmlBackupFiles.size(); i++)
        {
            xmlBackups.addItem(i + MenuFileXmlBackupMenuOffset, xmlBackupFiles[i].getFileName());
        }
        
        p.addSubMenu("Load XML Backup from Project", xmlBackups);


#if HISE_IOS
#else
		p.addSeparator();

		PopupMenu settingsSub;

		settingsSub.addCommandItem(mainCommandManager, MenuFileSettingsProject);
		settingsSub.addCommandItem(mainCommandManager, MenuFileSettingsUser);
		settingsSub.addCommandItem(mainCommandManager, MenuFileSettingsCompiler);
		settingsSub.addSeparator();
		settingsSub.addCommandItem(mainCommandManager, MenuFileSettingCheckSanity);
		settingsSub.addCommandItem(mainCommandManager, MenuFileSettingsCleanBuildDirectory);

		p.addSubMenu("Settings", settingsSub);


		PopupMenu exportSub;

        exportSub.addCommandItem(mainCommandManager, MenuExportFileAsPlugin);
		exportSub.addCommandItem(mainCommandManager, MenuExportFileAsEffectPlugin);
		exportSub.addCommandItem(mainCommandManager, MenuExportFileAsStandaloneApp);
		exportSub.addCommandItem(mainCommandManager, MenuExportFileAsPlayerLibrary);
        exportSub.addCommandItem(mainCommandManager, MenuExportFileAsSnippet);

		p.addSubMenu("Export", exportSub);
		p.addSeparator();
		ADD_ALL_PLATFORMS(MenuFileQuit);
#endif

		break; }
	case BackendCommandTarget::EditMenu:

		ADD_ALL_PLATFORMS(MenuEditUndo);
		ADD_ALL_PLATFORMS(MenuEditRedo);
		p.addSeparator();

        if(dynamic_cast<JavascriptCodeEditor*>(bpe->currentCopyPasteTarget.get()))
        {
            dynamic_cast<JavascriptCodeEditor*>(bpe->currentCopyPasteTarget.get())->addPopupMenuItems(p, nullptr);
            
        }
        else
        {
            ADD_ALL_PLATFORMS(MenuEditCopy);
            ADD_ALL_PLATFORMS(MenuEditPaste);
            p.addSeparator();

			ADD_ALL_PLATFORMS(MenuEditMoveUp);
			ADD_ALL_PLATFORMS(MenuEditMoveDown);
            
            const int chainOffset = 0x6000;
            
            ProcessorEditor * editor = dynamic_cast<ProcessorEditor*>(bpe->currentCopyPasteTarget.get());
            if(editor != nullptr)
            {
                for(int i = 0; i < editor->getProcessor()->getNumInternalChains(); i++)
                {
                    Processor *child =  editor->getProcessor()->getChildProcessor(i);
                    
                    p.addItem(chainOffset, "Show " + child->getId(), true, child->getEditorState(Processor::EditorState::Visible));
                }
            }
            
            ADD_ALL_PLATFORMS(MenuEditCreateScriptVariable);
			ADD_ALL_PLATFORMS(MenuEditCreateBase64State);
            ADD_ALL_PLATFORMS(MenuEditCloseAllChains);
            ADD_DESKTOP_ONLY(MenuEditPlotModulator);
        }
		break;
	case BackendCommandTarget::ToolsMenu:
	{
		p.addSectionHeader("Scripting Tools");
		ADD_ALL_PLATFORMS(MenuToolsCreateInterface);
		ADD_ALL_PLATFORMS(MenuToolsRecompile);
		ADD_ALL_PLATFORMS(MenuToolsCheckDuplicate);
		ADD_ALL_PLATFORMS(MenuToolsClearConsole);
		ADD_DESKTOP_ONLY(MenuToolsRecompileScriptsOnReload);
		ADD_DESKTOP_ONLY(MenuToolsSetCompileTimeOut);
		ADD_DESKTOP_ONLY(MenuToolsUseBackgroundThreadForCompile);
		ADD_DESKTOP_ONLY(MenuToolsCreateToolbarPropertyDefinition);
		ADD_DESKTOP_ONLY(MenuToolsCreateExternalScriptFile);
		ADD_DESKTOP_ONLY(MenuToolsValidateUserPresets);

		PopupMenu sub;

		Array<File> files;
		StringArray processors;

		bpe->getBackendProcessor()->fillExternalFileList(files, processors);

		for (int i = 0; i < files.size(); i++)
		{
			sub.addItem(MenuToolsExternalScriptFileOffset + i, processors[i] + ": " + files[i].getFileName());
		}

		p.addSubMenu("Edit external script files", sub, files.size() != 0);

		p.addSeparator();
		p.addSectionHeader("Sample Management");
		
		ADD_DESKTOP_ONLY(MenuToolsResolveMissingSamples);
		ADD_DESKTOP_ONLY(MenuToolsDeleteMissingSamples);
		ADD_DESKTOP_ONLY(MenuToolsUseRelativePaths);
		ADD_DESKTOP_ONLY(MenuToolsCheckAllSampleMaps);
		ADD_DESKTOP_ONLY(MenuToolsCollectExternalFiles);
		ADD_DESKTOP_ONLY(MenuToolsRedirectSampleFolder);
		ADD_DESKTOP_ONLY(MenuToolsForcePoolSearch);
		ADD_DESKTOP_ONLY(MenuToolsConvertAllSamplesToMonolith);
		ADD_DESKTOP_ONLY(MenuToolsConvertSfzToSampleMaps);
		ADD_DESKTOP_ONLY(MenuToolsEnableAutoSaving);
		ADD_DESKTOP_ONLY(MenuToolsEnableDebugLogging);
		p.addSeparator();
		p.addSectionHeader("License Management");
		ADD_DESKTOP_ONLY(MenuToolsCreateDummyLicenceFile);
		ADD_DESKTOP_ONLY(MenuToolsCreateRSAKeys);
		break;
	}
	case BackendCommandTarget::ViewMenu: {
		ADD_ALL_PLATFORMS(MenuViewBack);
		ADD_ALL_PLATFORMS(MenuViewForward);
		ADD_ALL_PLATFORMS(MenuViewReset);

		p.addSeparator();

		ADD_ALL_PLATFORMS(WorkspaceMain);
		ADD_ALL_PLATFORMS(WorkspaceScript);
		ADD_ALL_PLATFORMS(WorkspaceSampler);
		ADD_ALL_PLATFORMS(WorkspaceCustom);

		p.addSeparator();

		ADD_DESKTOP_ONLY(MenuViewEnableGlobalLayoutMode);
		ADD_DESKTOP_ONLY(MenuViewFullscreen);
		ADD_ALL_PLATFORMS(MenuViewShowSelectedProcessorInPopup);
		p.addSeparator();
		ADD_DESKTOP_ONLY(MenuOneColumn);
		ADD_DESKTOP_ONLY(MenuTwoColumns);
		ADD_DESKTOP_ONLY(MenuThreeColumns);
        p.addSeparator();
        ADD_ALL_PLATFORMS(MenuViewIncreaseCodeFontSize);
        ADD_ALL_PLATFORMS(MenuViewDecreaseCodeFontSize);
		ADD_ALL_PLATFORMS(Settings);
		
		break;
		}
	case BackendCommandTarget::HelpMenu:
			ADD_ALL_PLATFORMS(MenuHelpShowAboutPage);
			ADD_DESKTOP_ONLY(MenuHelpCheckVersion);
		break;
	default:
		break;
	}

	return p;
}



void BackendCommandTarget::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	if (topLevelMenuIndex == 1 && (dynamic_cast<JavascriptCodeEditor*>(bpe->currentCopyPasteTarget.get()) != nullptr))
	{
		dynamic_cast<JavascriptCodeEditor*>(bpe->currentCopyPasteTarget.get())->performPopupMenuAction(menuItemID);
		return;
	}

	if (menuItemID >= MenuOpenFileFromProjectOffset && menuItemID < ((int)(MenuOpenFileFromProjectOffset) + 50))
	{
        const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
        
        if (!shouldDiscard) return;
        
		if (GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive())
		{
			File f = recentFileList[menuItemID - (int)MenuOpenFileFromProjectOffset];

			if (f.existsAsFile())
			{
				bpe->mainEditor->loadNewContainer(f);
			}
		}
	}
	else if (menuItemID >= MenuProjectRecentOffset && menuItemID < (int)MenuProjectRecentOffset + 12)
	{
        const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
        
        if (!shouldDiscard) return;
        
		const int index = menuItemID - MenuProjectRecentOffset;

		if (PresetHandler::showYesNoWindow("Switch projects?", "Do you want to switch projects? The current preset will be cleared"))
		{
#if HISE_IOS
			Array<File> results;

			File userDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory);

			userDataDirectory.findChildFiles(results, File::findDirectories, false);

			File file = results[index];

			bpe->clearPreset();

			GET_PROJECT_HANDLER(bpe->getMainSynthChain()).setWorkingProject(file);

#else

			String file = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getRecentWorkDirectories()[index];

			bpe->mainEditor->clearPreset();

			GET_PROJECT_HANDLER(bpe->getMainSynthChain()).setWorkingProject(file, bpe);
            
            menuItemsChanged();
#endif
		}
	}
    else if (menuItemID >= MenuFileUserPresetMenuOffset && menuItemID < ((int)MenuFileUserPresetMenuOffset+50))
    {
        const int index = menuItemID - MenuFileUserPresetMenuOffset;
        
        Array<File> userPresetFiles;
		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getFileList(userPresetFiles, ProjectHandler::SubDirectories::UserPresets, "*.preset");

        File presetToLoad = userPresetFiles[index];
		Actions::loadUserPreset(bpe, presetToLoad);
    }
    else if (menuItemID >= MenuFileXmlBackupMenuOffset && menuItemID < ((int)MenuFileXmlBackupMenuOffset+50))
    {
        const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
        
        if (!shouldDiscard) return;
        
        const int index = menuItemID - MenuFileXmlBackupMenuOffset;
        
        Array<File> xmlFileList;
        
		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getFileList(xmlFileList, ProjectHandler::SubDirectories::XMLPresetBackups, "*.xml");
        
		File presetToLoad = xmlFileList[index];
            
        Actions::openFileFromXml(bpe, presetToLoad);
    }
    
	else if (menuItemID >= MenuViewOffset && menuItemID < (int)MenuViewOffset + 200)
	{
		ViewInfo *info = owner->synthChain->getViewInfo(menuItemID - MenuViewOffset);

		if (info != nullptr)
		{
			info->restore();

			bpe->mainEditor->setRootProcessor(info->getRootProcessor());

			owner->synthChain->setCurrentViewInfo(menuItemID - MenuViewOffset);
		}
	}
	else if (menuItemID >= MenuToolsExternalScriptFileOffset && menuItemID < (MenuToolsExternalScriptFileOffset + 50))
	{
		Array<File> files;
		StringArray processors;

		bpe->getBackendProcessor()->fillExternalFileList(files, processors);

		File f = files[menuItemID - MenuToolsExternalScriptFileOffset];

		Processor::Iterator<JavascriptProcessor> iter(bpe->getMainSynthChain());

		while (JavascriptProcessor *sp = iter.getNextProcessor())
		{
			for (int i = 0; i < sp->getNumWatchedFiles(); i++)
			{
				if (sp->getWatchedFile(i) == f)
				{
					sp->showPopupForFile(i);
					return;
				}
			}
		}

		jassertfalse;
	}
}



bool BackendCommandTarget::Actions::hasProcessorInClipboard()
{
	ScopedPointer<XmlElement> xml = XmlDocument::parse(SystemClipboard::getTextFromClipboard());

	if (xml != nullptr)
	{
		ValueTree v = ValueTree::fromXml(*xml);

		if (v.isValid() && v.getProperty("Type") == "SynthChain")
		{
			return true;
			
		}
	}

	return false;
}
                         
      
bool BackendCommandTarget::Actions::hasSnippetInClipboard()
{
    String clipboardContent = SystemClipboard::getTextFromClipboard();
 
    return clipboardContent.startsWith("HiseSnippet ");
}

void BackendCommandTarget::Actions::openFile(BackendRootWindow *bpe)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
#if HISE_IOS

	Array<File> fileList;

	GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Presets).findChildFiles(fileList, File::findFiles, false, "*.hip");

	MainMenuWithViewPort *menu = new MainMenuWithViewPort();

	menu->getContainer()->addFileIds(fileList, bpe);

	bpe->showPseudoModalWindow(menu, "Load File from Project", true);

#else

	FileChooser fc("Load Preset File", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Presets), "*.hip", true);

	if (fc.browseForFileToOpen()) bpe->loadNewContainer(fc.getResult());
#endif
}

void BackendCommandTarget::Actions::saveFile(BackendRootWindow *bpe, bool forceRename)
{
	if (forceRename || PresetHandler::showYesNoWindow("Save " + bpe->owner->getMainSynthChain()->getId(), "Do you want to save this preset?"))
	{
		const bool hasDefaultName = bpe->owner->getMainSynthChain()->getId() == "Master Chain";

		if (forceRename || hasDefaultName)
		{
			const String newName = PresetHandler::getCustomName("Preset");

			if (newName.isNotEmpty())
			{
				bpe->owner->getMainSynthChain()->setId(newName);
			}
			else return;
		}

		PresetHandler::saveProcessorAsPreset(bpe->owner->getMainSynthChain());
	}
}

void BackendCommandTarget::Actions::replaceWithClipboardContent(BackendRootWindow *bpe)
{
	String clipboardContent = SystemClipboard::getTextFromClipboard();

    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
	if (hasSnippetInClipboard())
	{
		String data = clipboardContent.fromFirstOccurrenceOf("HiseSnippet ", false, false);

		MemoryBlock mb;

		mb.fromBase64Encoding(data);

		MemoryInputStream mis(mb, false);

		GZIPDecompressorInputStream dezipper(&mis, false);

		ValueTree v = ValueTree::readFromGZIPData(mb.getData(), mb.getSize());

		if (v.isValid())
		{
			bpe->loadNewContainer(v);
			return;
		}
	}
	else
	{
		ScopedPointer<XmlElement> xml = XmlDocument::parse(clipboardContent);

		if (xml != nullptr)
		{
			ValueTree v = ValueTree::fromXml(*xml);

			if (v.isValid() && v.getProperty("Type") == "SynthChain")
			{
				bpe->loadNewContainer(v);
				return;
			}
		}
	}
    
	PresetHandler::showMessageWindow("Invalid Preset", "The clipboard does not contain a valid container / snippet.", PresetHandler::IconType::Warning);
}

void BackendCommandTarget::Actions::createScriptVariableDeclaration(CopyPasteTarget *currentCopyPasteTarget)
{
	ProcessorEditor *editor = dynamic_cast<ProcessorEditor*>(currentCopyPasteTarget);

	if (editor != nullptr)
	{
		ProcessorHelpers::getScriptVariableDeclaration(editor->getProcessor());
	}
}

void BackendCommandTarget::Actions::createBase64State(CopyPasteTarget* target)
{
	ProcessorEditor *editor = dynamic_cast<ProcessorEditor*>(target);

	if (editor != nullptr)
	{
		ProcessorHelpers::getBase64String(editor->getProcessor());
	}
}

void BackendCommandTarget::Actions::createUserInterface(BackendRootWindow * /*bpe*/)
{
	
}

void BackendCommandTarget::Actions::recompileAllScripts(BackendRootWindow * bpe)
{
	bpe->owner->compileAllScripts();
}

void BackendCommandTarget::Actions::toggleFullscreen(BackendRootWindow * bpe)
{
#if IS_STANDALONE_APP
    
    Component *window = bpe->getParentComponent()->getParentComponent();
    
    if (bpe->isFullScreenMode())
    {
        Desktop::getInstance().setKioskModeComponent(nullptr);
        
        const int height = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight() - 70;
        bpe->setSize(900, height);
        bpe->resized();
        
        window->centreWithSize(bpe->getWidth(), bpe->getHeight());
        
        bpe->setAlwaysOnTop(false);
        bpe->yBorderDragger->setVisible(true);
		bpe->xBorderDragger->setVisible(true);
    }
    else
    {
        Desktop::getInstance().setKioskModeComponent(window);
        
        bpe->yBorderDragger->setVisible(false);
		bpe->xBorderDragger->setVisible(false);
        bpe->setAlwaysOnTop(true);
        
        bpe->setSize(Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getWidth(),
                Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getHeight());
        
        bpe->resized();
        
    }
#else 
	ignoreUnused(bpe);
#endif
}

void BackendCommandTarget::Actions::addView(BackendRootWindow *bpe)
{
    String view = PresetHandler::getCustomName("View");
    
    ViewInfo *info = new ViewInfo(bpe->owner->synthChain, bpe->mainEditor->currentRootProcessor, view);
    
    bpe->owner->synthChain->addViewInfo(info);
}

void BackendCommandTarget::Actions::deleteView(BackendRootWindow *bpe)
{
    bpe->owner->synthChain->removeCurrentViewInfo();
}

void BackendCommandTarget::Actions::renameView(BackendRootWindow *bpe)
{
    String view = PresetHandler::getCustomName("View");
    
    bpe->owner->synthChain->getCurrentViewInfo()->setViewName(view);
}

void BackendCommandTarget::Actions::saveView(BackendRootWindow *bpe)
{
    String view = bpe->owner->synthChain->getCurrentViewInfo()->getViewName();
    
    ViewInfo *info = new ViewInfo(bpe->owner->synthChain, bpe->mainEditor->currentRootProcessor, view);
    
    bpe->owner->synthChain->replaceCurrentViewInfo(info);
}

void BackendCommandTarget::Actions::closeAllChains(BackendRootWindow *bpe)
{
    ProcessorEditor *editor = dynamic_cast<ProcessorEditor*>(bpe->currentCopyPasteTarget.get());
    
    if(editor != nullptr)
    {
        editor->getChainBar()->closeAll();
    }
}

void BackendCommandTarget::Actions::checkDuplicateIds(BackendRootWindow *bpe)
{
    PresetHandler::checkProcessorIdsForDuplicates(bpe->owner->synthChain, false);

}

void BackendCommandTarget::Actions::showAboutPage(BackendRootWindow * bpe)
{
	bpe->getMainTopBar()->togglePopup(MainTopBar::PopupType::About, true);

	//bpe->mainEditor->aboutPage->showAboutPage();
}

void BackendCommandTarget::Actions::checkVersion(BackendRootWindow *bpe)
{
    if (areMajorWebsitesAvailable())
    {
        UpdateChecker * checker = new UpdateChecker();
        
        checker->setModalBaseWindowComponent(bpe);
    }
    else
    {
        PresetHandler::showMessageWindow("Offline", "Could not connect to the server", PresetHandler::IconType::Warning);
    }
}

void BackendCommandTarget::Actions::setColumns(BackendRootWindow * bpe, BackendCommandTarget* target, ColumnMode columns)
{
	target->currentColumnMode = columns;

	switch (columns)
	{
	case BackendCommandTarget::OneColumn:
		bpe->setSize(900, bpe->getHeight());
		break;
	case BackendCommandTarget::TwoColumns:
		bpe->setSize(jmin<int>(Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getWidth(), 1280), bpe->getHeight());
		break;
	case BackendCommandTarget::ThreeColumns:
		bpe->setSize(jmin<int>(Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getWidth(), 1650), bpe->getHeight());
		break;
	default:
		break;
	}

	bpe->resized();
	
}

void BackendCommandTarget::Actions::plotModulator(CopyPasteTarget *currentCopyPasteTarget)
{
    ProcessorEditor *editor = dynamic_cast<ProcessorEditor*>(currentCopyPasteTarget);
                                                                         
    if(editor != nullptr)
    {
        Modulator *mod = dynamic_cast<Modulator*>(editor->getProcessor());
        if(mod != nullptr)
        {
            if(mod->isPlotted())
            {
                mod->getMainController()->removePlottedModulator(mod);
            }
            else
            {
                mod->getMainController()->addPlottedModulator(mod);
            }
        }
    }
}

void BackendCommandTarget::Actions::resolveMissingSamples(BackendRootWindow *bpe)
{
	bpe->getBackendProcessor()->getSampleManager().getModulatorSamplerSoundPool()->resolveMissingSamples(bpe);
}

void BackendCommandTarget::Actions::deleteMissingSamples(BackendRootWindow *bpe)
{
	bpe->getBackendProcessor()->getSampleManager().getModulatorSamplerSoundPool()->deleteMissingSamples();
}

void BackendCommandTarget::Actions::setCompileTimeOut(BackendRootWindow * bpe)
{
	AlertWindowLookAndFeel alaf;

	AlertWindow newTime("Enter new compile time out duration", "Current time out: " + String(bpe->getBackendProcessor()->getCompileTimeOut(),1) + " seconds.", AlertWindow::QuestionIcon, bpe);

	newTime.setLookAndFeel(&alaf);

	newTime.addTextEditor("time", String(bpe->getBackendProcessor()->getCompileTimeOut(), 1), "", false);
	
	newTime.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	newTime.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	if (newTime.runModalLoop())
	{
		double value = newTime.getTextEditor("time")->getText().getDoubleValue();

		if (value != 0.0)
		{
			bpe->getBackendProcessor()->setCompileTimeOut(value);
		}
	}
}

void BackendCommandTarget::Actions::toggleUseBackgroundThreadsForCompiling(BackendRootWindow * bpe)
{
	const bool lastState = bpe->getBackendProcessor()->isUsingBackgroundThreadForCompiling();

	bpe->getBackendProcessor()->setShouldUseBackgroundThreadForCompiling(!lastState);
}

void BackendCommandTarget::Actions::toggleCompileScriptsOnPresetLoad(BackendRootWindow * bpe)
{
	const bool lastState = bpe->getBackendProcessor()->isCompilingAllScriptsOnPresetLoad();

	bpe->getBackendProcessor()->setEnableCompileAllScriptsOnPresetLoad(!lastState);
}


void BackendCommandTarget::Actions::toggleRelativePath(BackendRootWindow * bpe)
{
	const bool state = bpe->getBackendProcessor()->getSampleManager().shouldUseRelativePathToProjectFolder();

	bpe->getBackendProcessor()->getSampleManager().setShouldUseRelativePathToProjectFolder(!state);
}


void BackendCommandTarget::Actions::collectExternalFiles(BackendRootWindow * bpe)
{
	ExternalResourceCollector *resource = new ExternalResourceCollector(bpe->getBackendProcessor());

	resource->setModalBaseWindowComponent(bpe);
}


void BackendCommandTarget::Actions::exportFileAsSnippet(BackendRootWindow* bpe)
{
	ValueTree v = bpe->getMainSynthChain()->exportAsValueTree();

	MemoryOutputStream mos;

	v.writeToStream(mos);

	MemoryOutputStream mos2;

	GZIPCompressorOutputStream zipper(&mos2, 9);
	
	zipper.write(mos.getData(), mos.getDataSize());
	zipper.flush();

	String data = "HiseSnippet " + mos2.getMemoryBlock().toBase64Encoding();

	SystemClipboard::copyTextToClipboard(data);

	PresetHandler::showMessageWindow("Preset copied as compressed snippet", "You can paste the clipboard content to share this preset", PresetHandler::IconType::Info);
}

void BackendCommandTarget::Actions::saveFileAsXml(BackendRootWindow * bpe)
{
	if (GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive())
	{
		FileChooser fc("Select XML file to save", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::XMLPresetBackups), "*.xml", true);

		if (fc.browseForFileToSave(true))
		{
			const bool hasDefaultName = bpe->owner->getMainSynthChain()->getId() == "Master Chain";

			if (hasDefaultName)
			{
				const String newName = fc.getResult().getFileNameWithoutExtension();
				bpe->owner->getMainSynthChain()->setId(newName);
			}

			ValueTree v = bpe->owner->getMainSynthChain()->exportAsValueTree();

            v.setProperty("BuildVersion", BUILD_SUB_VERSION, nullptr);
            
			ScopedPointer<XmlElement> xml = v.createXml();

			if(PresetHandler::showYesNoWindow("Strip Editor View information", "Do you want to strip the editor view information? This reduces the noise when using version controls systems."))
			{
				XmlBackupFunctions::removeEditorStatesFromXml(*xml);
			}

			if (PresetHandler::showYesNoWindow("Write Scripts to external file?", "Do you want to write the scripts into an external file (They will be imported when loading this backup"))
			{
				File scriptDirectory = XmlBackupFunctions::getScriptDirectoryFor(bpe->getMainSynthChain());

				Processor::Iterator<JavascriptProcessor> iter(bpe->getMainSynthChain());

				scriptDirectory.deleteRecursively();

				scriptDirectory.createDirectory();

				while (JavascriptProcessor *sp = iter.getNextProcessor())
				{
					if (sp->isConnectedToExternalFile())
						continue;

					String content;

					sp->mergeCallbacksToScript(content);

					File scriptFile = XmlBackupFunctions::getScriptFileFor(bpe->getMainSynthChain(), dynamic_cast<Processor*>(sp)->getId());

					scriptFile.replaceWithText(content);
				}

				XmlBackupFunctions::removeAllScripts(*xml);
			}

			fc.getResult().replaceWithText(xml->createDocument(""));

			debugToConsole(bpe->owner->getMainSynthChain(), "Exported as XML");
            
            
		}
	}
}

void BackendCommandTarget::Actions::openFileFromXml(BackendRootWindow * bpe, const File &fileToLoad)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
	if (GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive())
	{
		ScopedPointer<XmlElement> xml = XmlDocument::parse(fileToLoad);

		if (xml != nullptr)
		{
			String newId = xml->getStringAttribute("ID");

			XmlBackupFunctions::restoreAllScripts(*xml, bpe->getMainSynthChain(), newId);

			ValueTree v = ValueTree::fromXml(*xml);

			bpe->loadNewContainer(v);
		}
		else
		{
			PresetHandler::showMessageWindow("Corrupt File", "The XML file is not valid. Loading aborted", PresetHandler::IconType::Error);
		}
	}
}

void BackendCommandTarget::Actions::createNewProject(BackendRootWindow *bpe)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
	FileChooser fc("Create new project directory");

	if (fc.browseForDirectory())
	{
		File f = fc.getResult();

		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).createNewProject(f, bpe);

		bpe->getBackendProcessor()->createUserPresetData();
	}
}

void BackendCommandTarget::Actions::loadProject(BackendRootWindow *bpe)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
#if HISE_IOS

	Array<File> fileList;
	
	File::getSpecialLocation(File::userDocumentsDirectory).findChildFiles(fileList, File::findDirectories, false);


	MainMenuWithViewPort *newMenu = new MainMenuWithViewPort();

	newMenu->getContainer()->addFileIds(fileList, bpe);
	
	bpe->showPseudoModalWindow(newMenu, "Open a project from the list", true);

#else
	FileChooser fc("Load project (set as working directory)");

	if (fc.browseForDirectory())
	{
		File f = fc.getResult();

		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).setWorkingProject(f, bpe);

		bpe->getBackendProcessor()->createUserPresetData();
	}
#endif
}

void BackendCommandTarget::Actions::closeProject(BackendRootWindow *bpe)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
	GET_PROJECT_HANDLER(bpe->getMainSynthChain()).setWorkingProject(File(), bpe);


	bpe->getBackendProcessor()->createUserPresetData();

}

void BackendCommandTarget::Actions::showProjectInFinder(BackendRootWindow *bpe)
{
	if (GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive())
	{
		GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getWorkDirectory().revealToUser();
	}
}

void BackendCommandTarget::Actions::saveUserPreset(BackendRootWindow *bpe)
{
    UserPresetHandler::saveUserPreset(bpe->getMainSynthChain());
}

void BackendCommandTarget::Actions::loadUserPreset(BackendRootWindow *bpe, const File &fileToLoad)
{
    const bool shouldDiscard = !bpe->getBackendProcessor()->isChanged() || PresetHandler::showYesNoWindow("Discard the current preset?", "The current preset will be discarded", PresetHandler::IconType::Question);
    
    if (!shouldDiscard) return;
    
    UserPresetHandler::loadUserPreset(bpe->getMainSynthChain(), fileToLoad);
}

void BackendCommandTarget::Actions::redirectSampleFolder(Processor *processorForTheProjectHandler)
{
    FileChooser fc("Redirect sample folder to the following location");
    
    if (fc.browseForDirectory())
    {
        File f = fc.getResult();
        
		GET_PROJECT_HANDLER(processorForTheProjectHandler).createLinkFile(ProjectHandler::SubDirectories::Samples, f);
    }
}

void BackendCommandTarget::Actions::showFilePresetSettings(BackendRootWindow * /*bpe*/)
{
	
}

void BackendCommandTarget::Actions::showFileProjectSettings(BackendRootWindow * bpe)
{
	SettingWindows::ProjectSettingWindow *window = new SettingWindows::ProjectSettingWindow(&GET_PROJECT_HANDLER(bpe->getMainSynthChain()));

	window->setModalBaseWindowComponent(bpe);
}

void BackendCommandTarget::Actions::showFileUserSettings(BackendRootWindow * bpe)
{
	SettingWindows::UserSettingWindow *window = new SettingWindows::UserSettingWindow(&GET_PROJECT_HANDLER(bpe->getMainSynthChain()));

	window->setModalBaseWindowComponent(bpe);
}

void BackendCommandTarget::Actions::showFileCompilerSettings(BackendRootWindow * bpe)
{
	SettingWindows::CompilerSettingWindow *window = new SettingWindows::CompilerSettingWindow();

	window->setModalBaseWindowComponent(bpe);
}

void BackendCommandTarget::Actions::checkSettingSanity(BackendRootWindow * bpe)
{
	SettingWindows::checkAllSettings(&GET_PROJECT_HANDLER(bpe->getMainSynthChain()));
}

void BackendCommandTarget::Actions::togglePluginPopupWindow(BackendRootWindow * bpe)
{
	if (bpe->mainEditor->isPluginPreviewShown())
	{
		bpe->mainEditor->setPluginPreviewWindow(nullptr);
	}
	else
	{
#if HISE_IOS
		bpe->showPseudoModalWindow(new PluginPreviewWindow::Content(bpe), bpe->getMainSynthChain()->getId(), true);
#else
		bpe->mainEditor->setPluginPreviewWindow(new PluginPreviewWindow(bpe->mainEditor));
#endif
	}
}
                    
void BackendCommandTarget::Actions::changeCodeFontSize(BackendRootWindow *bpe, bool increase)
{
    float currentFontSize = bpe->getMainSynthChain()->getMainController()->getGlobalCodeFontSize();
    
    if(increase)
    {
        currentFontSize = jmin<float>(28.0f, currentFontSize + 1.0f);
    }
    else
    {
        currentFontSize = jmax<float>(10.0f, currentFontSize - 1.0f);
    }
    
    bpe->getMainSynthChain()->getMainController()->setGlobalCodeFontSize(currentFontSize);
}

void BackendCommandTarget::Actions::createRSAKeys(BackendRootWindow * bpe)
{
	GET_PROJECT_HANDLER(bpe->getMainSynthChain()).createRSAKey();
}


void BackendCommandTarget::Actions::createDummyLicenceFile(BackendRootWindow * bpe)
{
	ProjectHandler *handler = &GET_PROJECT_HANDLER(bpe->getMainSynthChain());

	if (!handler->isActive())
	{
		PresetHandler::showMessageWindow("No Project active", "You need an active project to create a key file.", PresetHandler::IconType::Warning);
		return;
	}

	const String appName = SettingWindows::getSettingValue((int)SettingWindows::ProjectSettingWindow::Attributes::Name, handler);

	const String version = SettingWindows::getSettingValue((int)SettingWindows::ProjectSettingWindow::Attributes::Version, handler);

	if (appName.isEmpty() || version.isEmpty())
	{
		PresetHandler::showMessageWindow("No Product name", "You need a product name for a licence file.", PresetHandler::IconType::Warning);
		return;
	}

	const String productName = appName + " " + version;

	const String dummyEmail = "dummy@email.com";
	const String userName = "Dummy McLovin";

	StringArray ids;

#if JUCE_WINDOWS
	OnlineUnlockStatus::MachineIDUtilities::addFileIDToList(ids, File::getSpecialLocation(File::windowsSystemDirectory));
#else
	OnlineUnlockStatus::MachineIDUtilities::addFileIDToList(ids, File("~"));
#endif

	// ..if that fails, use the MAC addresses..
	if (ids.size() == 0)
		OnlineUnlockStatus::MachineIDUtilities::addMACAddressesToList(ids);

	RSAKey privateKey = RSAKey(handler->getPrivateKey());

	if (!privateKey.isValid())
	{
		PresetHandler::showMessageWindow("No RSA key", "You have to create a RSA Key pair first.", PresetHandler::IconType::Warning);
		return;
	}

	String keyContent = KeyGeneration::generateKeyFile(productName, dummyEmail, userName, ids.joinIntoString("\n"), privateKey);
	File key = handler->getWorkDirectory().getChildFile(productName + ProjectHandler::Frontend::getLicenceKeyExtension());

	key.replaceWithText(keyContent);

#if JUCE_WINDOWS
#if JUCE_64BIT
	const String message = "A dummy licence file for 64bit plugins was created.\nTo load 32bit plugins, please use the 32bit version of HISE to create the licence file";
#else
	const String message = "A dummy licence file for 32bit plugins was created.\nTo load 64bit plugins, please use the 64bit version of HISE to create the licence file";
#endif
#else
	const String message = "A dummy licence file for the plugins was created.";
#endif

	PresetHandler::showMessageWindow("Licence File created", message, PresetHandler::IconType::Info);
}

void BackendCommandTarget::Actions::createDefaultToolbarJSON(BackendRootWindow * bpe)
{
	String json = DefaultFrontendBar::createJSONString(bpe->getBackendProcessor()->getToolbarPropertiesObject());

	String clipboard = "var toolbarData = ";
	
	clipboard << json;

	clipboard << ";\n\nContent.setToolbarProperties(toolbarData);";

	SystemClipboard::copyTextToClipboard(clipboard);

	PresetHandler::showMessageWindow("JSON Data copied to clipboard", 
		"The current toolbar properties are copied into the clipboard.\nPaste it into any script and change the data", 
		PresetHandler::IconType::Info);

}

void BackendCommandTarget::Actions::toggleForcePoolSearch(BackendRootWindow * bpe)
{
	ModulatorSamplerSoundPool *pool = bpe->getBackendProcessor()->getSampleManager().getModulatorSamplerSoundPool();

	pool->setForcePoolSearch(!pool->isPoolSearchForced());
}


void BackendCommandTarget::Actions::archiveProject(BackendRootWindow * bpe)
{
	ProjectHandler *handler = &GET_PROJECT_HANDLER(bpe->getMainSynthChain());

	if (handler->isRedirected(ProjectHandler::SubDirectories::Samples))
	{
		if (PresetHandler::showYesNoWindow("Sample Folder is redirected", 
										   "The sample folder is redirected to another location.\nIt will not be included in the archive. Press OK to continue or cancel to abort", 
										   PresetHandler::IconType::Warning))
			return;
	}

	FileChooser fc("Select archive destination", File(), "*.zip");

	if (fc.browseForFileToSave(true))
	{
		File archiveFile = fc.getResult();

		File projectDirectory = handler->getWorkDirectory();

		new ProjectArchiver(archiveFile, projectDirectory, bpe->getBackendProcessor());

	}
}

void BackendCommandTarget::Actions::downloadNewProject(BackendRootWindow * bpe)
{
	ProjectDownloader *downloader = new ProjectDownloader(bpe->mainEditor);

	downloader->setModalBaseWindowComponent(bpe);
}

void BackendCommandTarget::Actions::showMainMenu(BackendRootWindow * /*bpe*/)
{
#if 0
	const int commandIDs[] = { MenuNewFile,
		MenuOpenFile,
		MenuSaveFile,
		0,
		MenuReplaceWithClipboardContent,
		MenuExportFileAsSnippet,
		0,
		MenuFileDownloadNewProject,
		MenuProjectLoad,
		0,
		MenuEditCopy,
		MenuEditPaste,
		0,
		MainToolbarCommands::Settings};

	Array<int> ids;
	ids.addArray(commandIDs, numElementsInArray(commandIDs));

	MainMenuContainer *newMenu = new MainMenuContainer();

	newMenu->addCommandIds(bpe->mainCommandManager, ids);

	bpe->mainEditor->showPseudoModalWindow(newMenu, "Main Menu", true);
#endif

}

void BackendCommandTarget::Actions::moveModule(CopyPasteTarget *currentCopyPasteTarget, bool moveUp)
{
	if (ProcessorEditor *editor = dynamic_cast<ProcessorEditor*>(currentCopyPasteTarget))
	{
		Processor *processor = editor->getProcessor();
		ProcessorEditor *parentEditor = editor->getParentEditor();

		if (parentEditor == nullptr) return;

		if (Chain *c = parentEditor->getProcessorAsChain())
		{
			c->getHandler()->moveProcessor(editor->getProcessor(), moveUp ? -1 : 1);
			editor->childEditorAmountChanged();

			BackendRootWindow *bpe = editor->findParentComponentOfClass<BackendRootWindow>();
			bpe->mainEditor->refreshContainer(processor);
		}
	}
}

void BackendCommandTarget::Actions::createExternalScriptFile(BackendRootWindow * bpe)
{
	File scriptDirectory = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Scripts);

	String newScriptName = PresetHandler::getCustomName("Script File");

	if (newScriptName.isNotEmpty())
	{
		File newScriptFile = scriptDirectory.getChildFile(newScriptName + ".js");

		if (newScriptFile.exists())
		{
			PresetHandler::showMessageWindow("File already exists", "The file you are trying to create already exists", PresetHandler::IconType::Warning);
		}
		else
		{
			newScriptFile.create();
			newScriptFile.replaceWithText("/** External Script File " + newScriptName + " */\n\n");

			SystemClipboard::copyTextToClipboard("include(\"" + newScriptFile.getFileName() + "\");");

			PresetHandler::showMessageWindow("File created", "The file " + newScriptFile.getFullPathName() + " was successfully created.\nThe include statement was copied into the clipboard.");
		}
	}
}

void BackendCommandTarget::Actions::exportMainSynthChainAsPlayerLibrary(BackendRootWindow * bpe)
{
	
	FileChooser fc("Export as HisePlayerLibrary", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getWorkDirectory(), "*.hpl");

	if (fc.browseForFileToSave(true))
	{
		File s = fc.getResult();

		new HisePlayerExporter(bpe->getMainSynthChain(), s);
	}
	
	
}

void BackendCommandTarget::Actions::cleanBuildDirectory(BackendRootWindow * bpe)
{
	if (!GET_PROJECT_HANDLER(bpe->getMainSynthChain()).isActive()) return;

	File buildDirectory = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Binaries);

	if (buildDirectory.isDirectory())
	{
		const bool cleanEverything = PresetHandler::showYesNoWindow("Clean whole directory", "Do you want to delete all files in the build directory?\nPress Cancel to just delete the autogenerated IDE projects & include files", PresetHandler::IconType::Question);

		if (cleanEverything)
		{
			buildDirectory.deleteRecursively();
			buildDirectory.createDirectory();
		}
		else
		{
			buildDirectory.getChildFile("Builds").deleteRecursively();
			buildDirectory.getChildFile("JuceLibraryCode").deleteRecursively();
		}
	}
}

void BackendCommandTarget::Actions::convertAllSamplesToMonolith(BackendRootWindow * bpe)
{
	ModulatorSampler* sampler = dynamic_cast<ModulatorSampler*>(ProcessorHelpers::getFirstProcessorWithName(bpe->getMainSynthChain(), "Sampler"));

	if (sampler != nullptr)
	{
		MonolithConverter *converter = new MonolithConverter(bpe);

		converter->setModalBaseWindowComponent(bpe);
	}
	else
	{
		PresetHandler::showMessageWindow("Missing convert sampler", "You need a sampler with the name 'Sampler' in the Master Chain!", PresetHandler::IconType::Error);
	}
}

void BackendCommandTarget::Actions::convertSfzFilesToSampleMaps(BackendRootWindow * bpe)
{
	ModulatorSampler* sampler = dynamic_cast<ModulatorSampler*>(ProcessorHelpers::getFirstProcessorWithName(bpe->getMainSynthChain(), "Sampler"));

	if (sampler != nullptr)
	{
		FileChooser fc("Select SFZ files to convert", GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Samples), "*.sfz;*.SFZ");

		if (fc.browseForMultipleFilesToOpen())
		{
			auto files = fc.getResults();

			for (auto f : files)
			{
				SfzImporter importer(sampler, f);

				importer.importSfzFile();

				const String id = f.getFileNameWithoutExtension();

				sampler->getSampleMap()->setId(f.getFileNameWithoutExtension());
				ValueTree sampleMap = sampler->getSampleMap()->exportAsValueTree();

				File sampleMapFile = GET_PROJECT_HANDLER(sampler).getSubDirectory(ProjectHandler::SubDirectories::SampleMaps).getChildFile(id + ".xml");

				ScopedPointer<XmlElement> xml = sampleMap.createXml();
				xml->writeToFile(sampleMapFile, "");
			}
		}
	}
	else
	{
		PresetHandler::showMessageWindow("Missing convert sampler", "You need a sampler with the name 'Sampler' in the Master Chain!", PresetHandler::IconType::Error);
	}
}

void BackendCommandTarget::Actions::checkAllSamplemaps(BackendRootWindow * bpe)
{
	GET_PROJECT_HANDLER(bpe->getMainSynthChain()).checkAllSampleMaps();
}

void removeHiddenFilesFromList(Array<File> &list)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].isHidden() || list[i].getFileName().startsWith("."))
		{
			list.remove(i--);
		}
	}
}

void BackendCommandTarget::Actions::validateUserPresets(BackendRootWindow * bpe)
{
	MainController* mc = bpe->getBackendProcessor();
	File userPresetDirectory = GET_PROJECT_HANDLER(mc->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::UserPresets);
	Array<File> userPresets;
	userPresetDirectory.findChildFiles(userPresets, File::findFiles, true, "*.preset");
	removeHiddenFilesFromList(userPresets);

	if (PresetHandler::showYesNoWindow("Update for missing controls", "Do you want to add default values if controls are missing?"))
	{
		int numChangedPresets = 0;
		int maxNumChangedControls = 0;

		for (auto f : userPresets)
		{
			const int numControlsForThisFile = UserPresetHandler::addMissingControlsToUserPreset(mc->getMainSynthChain(), f);

			if (numControlsForThisFile == -1)
			{
				PresetHandler::showMessageWindow("Error at validating", "Aborting...", PresetHandler::IconType::Error);
			}

			if (numControlsForThisFile > 0)
			{
				maxNumChangedControls = jmax<int>(maxNumChangedControls, numControlsForThisFile);
				numChangedPresets++;
			}
		}

		if (numChangedPresets != 0)
			PresetHandler::showMessageWindow("Presets changed", String(numChangedPresets) + " user presets were modified and up to " + String(maxNumChangedControls) + " controls were added.", PresetHandler::IconType::Info);

		int numWrongVersionedFiles = 0;

		if (PresetHandler::showYesNoWindow("Validate version", "Do you want to check / update the version", PresetHandler::IconType::Question))
		{
			for (auto f : userPresets)
			{
				if (UserPresetHandler::updateVersionNumber(mc->getMainSynthChain(), f))
					numWrongVersionedFiles++;
			}
		}

		if (numWrongVersionedFiles != 0)
			PresetHandler::showMessageWindow("Version updated", String(numWrongVersionedFiles) + " user presets were updated to the recent version", PresetHandler::IconType::Info);

		if (numChangedPresets == 0 && numWrongVersionedFiles == 0)
			PresetHandler::showMessageWindow("Nothing to do", "All user presets are up to date.");
	}
}



#undef ADD_ALL_PLATFORMS
#undef ADD_IOS_ONLY
#undef ADD_DESKTOP_ONLY
#undef toggleVisibility
